/**
 * @file src_input_remote.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <src_input_remote.hpp>
#include <curl/curl.h>
#include <archive.h>
#include <archive_entry.h>

#ifdef __MACH__
#include <sys/time.h>
#define CLOCK_REALTIME 0
//clock_gettime is not implemented on OSX
int clock_gettime(int /*clk_id*/, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

namespace {
    struct curl {
        CURL* handle;
        CURLM* multi_handle;
        CURLMsg* msg;
        int msgs_left;
        int still_running;
        size_t data_len;
        char* data_buffer;
        std::string source;
    };
}

size_t curl_cb(void* buffer, size_t len, size_t nmemb, void* data);

int     archive_curl_open(archive *, void *client_data);
ssize_t archive_curl_read(archive *, void *client_data, const void **buff);
int     archive_curl_close(archive *, void *client_data);

void src_input_remote(ParseQueue& queue,
                      srcml_archive* srcml_arch,
                      const std::string& remote_uri,
                      const boost::optional<std::string>& option_language,
                      const boost::optional<std::string>& option_filename,
                      const boost::optional<std::string>& option_directory,
                      const boost::optional<std::string>& option_version) {

    archive* arch = archive_read_new();

    archive_read_support_format_ar(arch);
    archive_read_support_format_cpio(arch);
    archive_read_support_format_gnutar(arch);
    archive_read_support_format_iso9660(arch);
    archive_read_support_format_mtree(arch);
    archive_read_support_format_tar(arch);
    archive_read_support_format_xar(arch);
    archive_read_support_format_zip(arch);
    archive_read_support_format_raw(arch);
    archive_read_support_format_empty(arch);

    /* Check libarchive version enable version specific features/syntax */
#if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(arch);
#else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(arch);
    archive_read_support_format_cab(arch);
    archive_read_support_format_lha(arch);
    archive_read_support_format_rar(arch);

    // Compressions
    archive_read_support_filter_all(arch);
#endif

    curl curling;
    curling.source = remote_uri;
    int status = archive_read_open(arch, &curling, archive_curl_open, archive_curl_read, archive_curl_close);
    if (status == ARCHIVE_OK) {

        /* In general, go through this once for each time the header can be read
           Exception: if empty, go through the loop exactly once */
        int count = 0;
        int status = ARCHIVE_OK;
        archive_entry *entry;
        while (status == ARCHIVE_OK &&
               (((status = archive_read_next_header(arch, &entry)) == ARCHIVE_OK) ||
                (status == ARCHIVE_EOF && !count))) {

            // skip any directories
            if (status == ARCHIVE_OK && archive_entry_filetype(entry) == AE_IFDIR)
                continue;

            // default is filename from archive entry (if not empty)
            std::string filename = status == ARCHIVE_OK ? archive_entry_pathname(entry) : "";

            if (count && filename != "data")
                srcml_archive_enable_option(srcml_arch, SRCML_OPTION_ARCHIVE);

            // archive entry filename for non-archive input is "data"
            if (filename.empty() || filename == "data")
                filename = remote_uri;

            if (option_filename)
                filename = *option_filename;

            // language may have been explicitly set
            std::string language;

            if (option_language)
                language = *option_language;

            // if not explicitly set, language comes from extension
            // we have to do this ourselves, since libsrcml won't for memory
            if (language.empty())
                if (const char* l = srcml_archive_check_extension(srcml_arch, filename.c_str()))
                    language = l;

            // form the parsing request
            ParseRequest request;
            if (option_filename || filename != "-")
                request.filename = filename;
            request.directory = option_directory;
            request.version = option_version;
            request.srcml_arch = srcml_arch;
            request.language = language;
            request.status = !language.empty() ? 0 : SRCML_STATUS_UNSET_LANGUAGE;

            // fill up the parse request buffer
            request.buffer.clear();
            if (!status) {
                const char* buffer;
                size_t size;
                int64_t offset;
                while (status == ARCHIVE_OK && archive_read_data_block(arch, (const void**) &buffer, &size, &offset) == ARCHIVE_OK)
                    request.buffer.insert(request.buffer.end(), buffer, buffer + size);
            }

            // Hand request off to the processing queue
            queue.push(request);

            ++count;
        }
        archive_read_finish(arch);
    }
}

size_t curl_cb(void* buffer, size_t len, size_t nmemb, void* data) {

    curl *curldata = (curl*) data;

    curldata->data_buffer = (char*)buffer;
    curldata->data_len = len * nmemb;

    return curldata->data_len;
}

int archive_curl_open(archive*, void* client_data) {

    curl* curldata = (curl*) client_data;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curldata->handle = curl_easy_init();

    curl_easy_setopt(curldata->handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curldata->handle, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curldata->handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
    curl_easy_setopt(curldata->handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curldata->handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curldata->handle, CURLOPT_WRITEFUNCTION, curl_cb);
    curl_easy_setopt(curldata->handle, CURLOPT_WRITEDATA, curldata);
    curl_easy_setopt(curldata->handle, CURLOPT_URL, curldata->source.c_str());

    curldata->multi_handle = curl_multi_init();
    curl_multi_add_handle(curldata->multi_handle, curldata->handle);
    curl_multi_perform(curldata->multi_handle, &curldata->still_running);

    // TODO: SOMETHING HERE TO MAKE SURE THE FILE IS ACTUALLY PRESENT
    return ARCHIVE_OK;
}

ssize_t archive_curl_read(archive*, void* client_data, const void** buff) {

    curl* curldata = (curl*) client_data;

    curldata->data_len = 0;

    while(curldata->data_len == 0 && curldata->still_running) {
        curl_multi_perform(curldata->multi_handle, &curldata->still_running);
    }

    *buff = curldata->data_buffer;

    return curldata->data_len;
}

int archive_curl_close(archive*, void* client_data) {

    curl* curldata = (curl*) client_data;

    while ((curldata->msg = curl_multi_info_read(curldata->multi_handle, &curldata->msgs_left))) {
        if (curldata->msg->msg == CURLMSG_DONE) {
            if (curldata->msg->data.result) {
//                std::cerr << "Download status: " << curldata->msg->data.result << "\n";
            }
            break;
        }
    }

    curl_multi_cleanup(curldata->multi_handle);
    curl_easy_cleanup(curldata->handle);

    return 0;
}
