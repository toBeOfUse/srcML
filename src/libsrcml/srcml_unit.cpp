/**
 * @file srcml_unit.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_translator.hpp>
#include <srcml_sax2_reader.hpp>
#include <UTF8CharBuffer.hpp>
#include <memory>

/******************************************************************************
 *                                                                            *
 *                           Set up functions                                 *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unit_set_src_encoding
 * @param unit a srcml unit
 * @param encoding an encoding
 *
 * Set the encoding for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_set_src_encoding(struct srcml_unit* unit, const char* encoding) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->encoding = encoding ? std::string(encoding) : decltype(unit->encoding)();

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_set_language
 * @param unit a srcml unit
 * @param language a language
 *
 * Set the language for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_set_language(struct srcml_unit* unit, const char* language) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->language = language ? std::string(language) : decltype(unit->language)();

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_set_filename
 * @param unit a srcml unit
 * @param filename the name of a file
 *
 * Set the filename attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_set_filename(struct srcml_unit* unit, const char* filename) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->filename = filename ? std::string(filename) : decltype(unit->filename)();

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_set_version
 * @param unit a srcml unit
 * @param version a version string
 *
 * Set the version attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_set_version(struct srcml_unit* unit, const char* version) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->version = version ? std::string(version) : decltype(unit->version)();

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_set_timestamp
 * @param unit a srcml unit
 * @param timestamp a timestamp string
 *
 * Set the timestamp attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_set_timestamp(struct srcml_unit* unit, const char* timestamp) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->timestamp = timestamp ? std::string(timestamp) : decltype(unit->timestamp)();

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_set_hash
 * @param unit a srcml unit
 * @param hash a hash string
 *
 * Set the hash attribute for the srcml unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_set_hash(struct srcml_unit* unit, const char* hash) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->hash = hash ? std::string(hash) : decltype(unit->hash)();

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_unparse_set_eol
 * @param unit a srcml unit
 * @param eol the kind of eol to use for unparse
 *
 * Set the eol to be used for unparse.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unit_unparse_set_eol(struct srcml_unit* unit, size_t eol) {

    if (unit == nullptr || eol > SOURCE_OUTPUT_EOL_CRLF)
        return SRCML_STATUS_INVALID_ARGUMENT;

    unit->eol = eol;

    return SRCML_STATUS_OK;
}

/******************************************************************************
 *                                                                            *
 *                           Accessor functions                               *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unit_get_src_encoding
 * @param unit a srcml unit
 *
 * Get the encoding for the srcml unit
 *
 * @returns encoding on success and NULL on failure.
 */
const char* srcml_unit_get_src_encoding(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->encoding);
}

/**
 * srcml_unit_get_revision
 * @param unit a srcml unit
 *
 * Get the revision for the srcml unit
 *
 * @returns revision on success and NULL on failure.
 */
const char* srcml_unit_get_revision(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->revision);
}

/**
 * srcml_unit_get_language
 * @param unit a srcml unit
 *
 * Get the language for the srcml unit
 *
 * @returns language on success and NULL on failure.
 */
const char* srcml_unit_get_language(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->language);
}

/**
 * srcml_unit_get_filename
 * @param unit a srcml unit
 *
 * Get the filename attribute for the srcml unit.
 *
 * @returns filename attribute on success and NULL on failure.
 */
const char* srcml_unit_get_filename(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->filename);
}

/**
 * srcml_unit_get_version
 * @param unit a srcml unit
 *
 * Get the version for the srcml unit.
 *
 * @returns version on success and NULL on failure.
 */
const char* srcml_unit_get_version(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->version);
}

/**
 * srcml_unit_get_timestamp
 * @param unit a srcml unit
 *
 * Get the timestamp for the srcml unit.
 *
 * @returns timestamp on success and NULL on failure.
 */
const char* srcml_unit_get_timestamp(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->timestamp);
}

/**
 * srcml_unit_get_hash
 * @param unit a srcml unit
 *
 * Get the hash for the srcml unit.
 *
 * @returns hash on success and NULL on failure.
 */
const char* srcml_unit_get_hash(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    return optional_to_c_str(unit->hash);
}

/**
 * srcml_unit_get_xml_fragment
 * @param unit a srcml unit
 *
 * Get the parsed or collected srcml from an archive.
 * If only the attributes were collected from a read,
 * then read in the xml and return that value.  XML fragment returned
 * is the raw UTF-8 encoded XML stored internally and is not completely XML.
 * Do not free.
 *
 * @returns the raw unit srcML on success and NULL on failure.
 */
const char* srcml_unit_get_xml_fragment(struct srcml_unit* unit) {

    if (unit == nullptr || (!unit->read_body && !unit->read_header))
        return 0;

    if (!unit->read_body && (unit->archive->type == SRCML_ARCHIVE_READ || unit->archive->type == SRCML_ARCHIVE_RW))
        unit->archive->reader->read_body(unit);

    return optional_to_c_str(unit->srcml);
}

/**
 * srcml_unit_get_xml_standalone
 * @param unit a srcml unit
 * @param xml_encoding the xml encoding to encode the unit
 * @param xml_buffer buffer to return the standalone xml
 * @param buffer_size the size of the returned buffer
 *
 * Get the parsed or collected srcml from an archive.
 * If only the attributes were collected from a read,
 * then read in the xml and return that value.  XML returned
 * is formatted version of the internally stored xml after
 * applying encoding, and appending of namespaces.  It is a complete standalone XML.
 * Must free when done using.
 *
 * @returns the formatted unit srcML on success and NULL on failure.
 */
int srcml_unit_get_xml_standalone(struct srcml_unit* unit, const char* xml_encoding, char** xml_buffer, size_t* buffer_size) {

    if (unit == nullptr || xml_buffer == nullptr || buffer_size == nullptr || (!unit->read_body && !unit->read_header))
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (!unit->read_body && (unit->archive->type == SRCML_ARCHIVE_READ || unit->archive->type == SRCML_ARCHIVE_RW))
        unit->archive->reader->read_body(unit);

    if (!unit->read_body)
        return SRCML_STATUS_ERROR;

    *xml_buffer = 0;
    *buffer_size = 0;

    srcml_archive* formatting_archive = srcml_archive_clone(unit->archive);
    srcml_archive_disable_option(formatting_archive, SRCML_OPTION_ARCHIVE);
    if (xml_encoding)
        srcml_archive_set_xml_encoding(formatting_archive, xml_encoding);
    srcml_archive_write_open_memory(formatting_archive, xml_buffer, buffer_size);
    srcml_archive_write_unit(formatting_archive, unit);
    srcml_archive_close(formatting_archive);
    srcml_archive_free(formatting_archive);

    // @todo Don't think this is necessary
    while (*buffer_size > 0 && (*xml_buffer)[*buffer_size - 1] == '\n')
        (*xml_buffer)[--(*buffer_size)] = '\0';

    return SRCML_STATUS_OK;
}

/******************************************************************************
 *                                                                            *
 *                           Unit parsing functions                           *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unit_parse_internal
 * @param unit a srcml unit
 * @param lang an interger representation of a language
 * @param input the source input to the translator
 * @param translation_options the options for translation
 *
 * Function for internal use for parsing functions. Creates
 * output buffer, translates a current input and places the
 * contents into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_IO_ERROR on failure.
 */
static int srcml_unit_parse_internal(struct srcml_unit* unit, const char* filename,
    std::function<UTF8CharBuffer*(const char* src_encoding, bool output_hash, boost::optional<std::string>& hash)> createUTF8CharBuffer) {

    // @todo Figure out what the state of the archive should be
//    if (unit->archive->type != SRCML_ARCHIVE_WRITE && unit->archive->type != SRCML_ARCHIVE_RW)
//        return SRCML_STATUS_INVALID_IO_OPERATION;

    // figure out the language based on unit, archive, registered languages
    int lang = unit->language ? srcml_check_language(unit->language->c_str())
        : (unit->archive->language ? srcml_check_language(unit->archive->language->c_str()) : SRCML_LANGUAGE_NONE);

    if (lang == SRCML_LANGUAGE_NONE && filename)
        lang = unit->archive->registered_languages.get_language_from_filename(filename);

    if (lang == SRCML_LANGUAGE_NONE)
        return SRCML_STATUS_UNSET_LANGUAGE;

    unit->derived_language = lang;

    if (*unit->language == "C++" || *unit->language == "C" || *unit->language == "Objective-C" || *unit->language == "C#")
        unit->archive->options |= SRCML_OPTION_CPP;

    const char* src_encoding = optional_to_c_str(unit->encoding, optional_to_c_str(unit->archive->src_encoding));
    bool output_hash = !unit->hash && unit->archive->options & SRCML_OPTION_HASH;

    UTF8CharBuffer* input = 0;
    try {

        input = createUTF8CharBuffer(src_encoding, output_hash, unit->hash);

    } catch(...) { return SRCML_STATUS_IO_ERROR; }

    unit->encoding = input->getEncoding();

    // if this is just a solitary unit (i.e., no archive), the url attribute is on the unit
    if (!srcml_archive_is_full_archive(unit->archive))
        unit->url = unit->archive->url;

    // create the unit start tag
    int status = srcml_write_start_unit(unit);
    if (status != SRCML_STATUS_OK)
        return status;

    // record start of content (after the unit start tag)
    xmlTextWriterFlush(unit->unit_translator->output_textwriter());
    unit->content_begin = unit->unit_translator->output_buffer()->written + 1;

    // parse the input
    unit->unit_translator->translate(input);

    // record end of content (before the unit end tag)
    xmlTextWriterFlush(unit->unit_translator->output_textwriter());
    unit->content_end = unit->unit_translator->output_buffer()->written + 1;

    // namespaces were updated during translation, may now include 
    // namespaces that were optional
    unit->namespaces = unit->unit_translator->out.getNamespaces();

    // create the unit end tag
    status = srcml_write_end_unit(unit);
    if (status != SRCML_STATUS_OK)
        return status;

    unit->read_body = true;

    return status;
}

/**
 * srcml_unit_parse_filename
 * @param unit a unit to parse the results to
 * @param src_filename name of a file to parse into srcML
 *
 * Convert to srcML the contents of src_filename and
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_parse_filename(struct srcml_unit* unit, const char* src_filename) {

    if (unit == nullptr || src_filename == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_parse_internal(unit, src_filename, [src_filename](const char* encoding, bool output_hash, boost::optional<std::string>& hash)-> UTF8CharBuffer* {

        return new UTF8CharBuffer(src_filename, encoding, output_hash, hash);
    });
}

/**
 * srcml_unit_parse_memory
 * @param unit a unit to parse the results to
 * @param src_buffer buffer containing source code to parse into srcML
 * @param buffer_size size of the buffer to parse
 *
 * Convert to srcML the contents of buffer up to size buffer_size and
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_parse_memory(struct srcml_unit* unit, const char* src_buffer, size_t buffer_size) {

    if (unit == nullptr || (buffer_size && src_buffer == nullptr))
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_parse_internal(unit, 0, [src_buffer, buffer_size](const char* encoding, bool output_hash, boost::optional<std::string>& hash)-> UTF8CharBuffer* {

        return new UTF8CharBuffer(src_buffer ? src_buffer : "", buffer_size, encoding, output_hash, hash);
    });
}

/**
 * srcml_unit_parse_FILE
 * @param unit a unit to parse the results to
 * @param src_file a FILE opened for reading
 *
 * Convert to srcML the contents of src_file and
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_parse_FILE(struct srcml_unit* unit, FILE* src_file) {

    if (unit == nullptr || src_file == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_parse_internal(unit, 0, [src_file](const char* encoding, bool output_hash, boost::optional<std::string>& hash)-> UTF8CharBuffer* {

        return new UTF8CharBuffer(src_file, encoding, output_hash, hash);
    });
}

/**
 * srcml_unit_parse_fd
 * @param unit a unit to parse the results to
 * @param src_fd a file descriptor open for reading
 *
 * Convert to srcML the contents of src_fd
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_parse_fd(struct srcml_unit* unit, int src_fd) {

    if (unit == nullptr || src_fd < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_parse_internal(unit, 0, [src_fd](const char* encoding, bool output_hash, boost::optional<std::string>& hash)-> UTF8CharBuffer* {

        return new UTF8CharBuffer(src_fd, encoding, output_hash, hash);
    });
}

/**
 * srcml_unit_parse_io
 * @param unit a unit to parse the results to
 * @param context an io context
 * @param read_callback a read callback function
 * @param close_callback a close callback function
 *
 * Convert to srcML the contents from the opened context
 * accessed via read_callback and closed via close_callback
 * place it into the unit.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_parse_io(struct srcml_unit* unit, void * context, ssize_t (*read_callback)(void * context, void * buffer, size_t len), int (*close_callback)(void * context)) {

    if (unit == nullptr || context == nullptr || read_callback == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_parse_internal(unit, 0, [context, read_callback, close_callback](const char* encoding, bool output_hash, boost::optional<std::string>& hash)-> UTF8CharBuffer* {

        return new UTF8CharBuffer(context, read_callback, close_callback, encoding, output_hash, hash);
    });
}

/******************************************************************************
 *                                                                            *
 *                           Unit unparsing functions                         *
 *                                                                            *
 ******************************************************************************/

static int srcml_unit_unparse_internal(struct srcml_unit* unit, std::function<xmlOutputBufferPtr(xmlCharEncodingHandlerPtr)> createbuffer) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->archive->type != SRCML_ARCHIVE_READ && unit->archive->type != SRCML_ARCHIVE_RW)
        return SRCML_STATUS_INVALID_IO_OPERATION;

    if (!unit->read_body && !unit->read_header)
        return SRCML_STATUS_UNINITIALIZED_UNIT;

    const char* encoding = optional_to_c_str(unit->encoding, optional_to_c_str(unit->archive->src_encoding, "ISO-8859-1"));

    xmlOutputBufferPtr output_handler = createbuffer(encoding ? xmlFindCharEncodingHandler(encoding) : 0);
    if (!output_handler) {
        return SRCML_STATUS_IO_ERROR;
    }

    int status = -1;
    try {

        if (!unit->read_body)
            unit->archive->reader->read_body(unit);

        status = SRCML_STATUS_OK;

    } catch(...) {

        status = SRCML_STATUS_IO_ERROR;
    }

    xmlOutputBufferWrite(output_handler, (int) unit->src.size(), unit->src.c_str());

    xmlOutputBufferClose(output_handler);

    return status;
}

/**
 * srcml_unit_unparse_filename
 * @param unit a srcml unit
 * @param src_filename name of a file to output contents of unit as source
 *
 * Convert the srcML in unit into source code and place it into the file
 * src_filename.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_unparse_filename(struct srcml_unit* unit, const char* src_filename, unsigned short /* compression */) {

    if (unit == nullptr || src_filename == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_unparse_internal(unit, [src_filename](xmlCharEncodingHandlerPtr handler) {

        return xmlOutputBufferCreateFilename(src_filename, handler, 0);
    });
}

/**
 * srcml_unit_unparse_memory
 * @param unit a srcml unit
 * @param src_buffer an output buffer address
 * @param src_size the size of the resulting buffer
 *
 * Convert the srcML in unit into source code and place it into the address
 * pointed to by src_buffer.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.  src_buffer is allocated in the function and
 * needs to be freed after finished using.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_unparse_memory(struct srcml_unit* unit, char** src_buffer, size_t * src_size) {

    if (unit == nullptr || src_buffer == nullptr || src_size == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    xmlBufferPtr buffer = nullptr;
    int status = srcml_unit_unparse_internal(unit, [&buffer](xmlCharEncodingHandlerPtr handler) {

        buffer = xmlBufferCreate();
        return xmlOutputBufferCreateBuffer(buffer, handler);
    });
    if (status != SRCML_STATUS_OK)
        return status;

    (*src_buffer) = (char *)buffer->content;
    buffer->content = 0;
    if (!buffer->content && !(*src_buffer))
        return SRCML_STATUS_ERROR;
    *src_size = strlen(*src_buffer);

    return SRCML_STATUS_OK;
}

/**
 * srcml_unit_unparse_FILE
 * @param unit a srcml unit
 * @param srcml_file FILE opened for writing
 *
 * Convert the srcML in unit into source code and place it into the FILE
 * srcml_file.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_unparse_FILE(struct srcml_unit* unit, FILE* srcml_file) {

    if (unit == nullptr || srcml_file == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_unparse_internal(unit, [srcml_file](xmlCharEncodingHandlerPtr handler) {

        return xmlOutputBufferCreateFile(srcml_file, handler);
    });
}

/**
 * srcml_unit_unparse_fd
 * @param unit a srcml unit
 * @param srcml_fd file descriptor opened for writing
 *
 * Convert the srcML in unit into source code and place it into the file
 * descriptor srcml_fd.  If the srcML was not read in, but the attributes were
 * read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_unparse_fd(struct srcml_unit* unit, int srcml_fd) {

    if (unit == nullptr || srcml_fd < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_unparse_internal(unit, [srcml_fd](xmlCharEncodingHandlerPtr handler) {

        return xmlOutputBufferCreateFd(srcml_fd, handler);
    });
}

/**
 * srcml_unit_unparse_io
 * @param unit a srcml unit
 * @param write_callback a write callback function
 * @param close_callback a close callback function
 *
 * Convert the srcML in unit into source code and place it into 
 * the opened io context written to using write callback
 * and closed using close callback.  If the srcML was not read in,
 * but the attributes were read in the xml and unparse that value.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_unit_unparse_io(struct srcml_unit* unit, void* context, int (*write_callback)(void* context, const char* buffer, size_t len), int (*close_callback)(void* context)) {

    if (unit == nullptr || context == nullptr || write_callback == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return srcml_unit_unparse_internal(unit, [write_callback, close_callback, context](xmlCharEncodingHandlerPtr handler) {

        return xmlOutputBufferCreateIO((int (*const)(void *, const char *, int))write_callback, close_callback, context, handler);
    });
}

/**
 * srcml_write_start_unit
 * @param archive a srcml archive opened for writing
 * @param unit a srcml_unit to start output with
 *
 * Begin by element output and output the start tag unit to the srcml_archive archive
 * using attributes in srcml_unit.
 *
 * Can not usage with add_unit call srcml_write_end_unit, to end element mode.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_start_unit(struct srcml_unit* unit) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // setup the output buffer where the srcML will be created
    std::unique_ptr<xmlBuffer> output_buffer(xmlBufferCreate());
    if (!output_buffer)
        return SRCML_STATUS_IO_ERROR;

    xmlOutputBufferPtr obuffer = xmlOutputBufferCreateBuffer(output_buffer.get(), xmlFindCharEncodingHandler("UTF-8"));
    if (!obuffer)
        return SRCML_STATUS_IO_ERROR;

    // setup the translator (srcML parser + srcML output)
    try {
        // turn off option for archive so XML generated has full namespaces
        auto options = unit->archive->options;
        options &= ~(unsigned long long)(SRCML_OPTION_ARCHIVE);
    
        unit->unit_translator = new srcml_translator(
            obuffer,
            optional_to_c_str(unit->archive->encoding, "UTF-8"),
            options,
            unit->archive->namespaces,
            boost::none,
            unit->archive->tabstop,
            unit->derived_language,
            optional_to_c_str(unit->revision),
            optional_to_c_str(unit->url),
            optional_to_c_str(unit->filename),
            optional_to_c_str(unit->version),
            unit->attributes,
            optional_to_c_str(unit->timestamp),
            optional_to_c_str(unit->hash, (unit->archive->options & SRCML_OPTION_HASH ? "" : 0)),
            optional_to_c_str(unit->encoding));

        unit->unit_translator->set_macro_list(unit->archive->user_macro_list);

    } catch(...) {

        return SRCML_STATUS_IO_ERROR;
    }

    // create the unit start tag
    if (!unit->unit_translator->add_start_unit(unit))
        return SRCML_STATUS_INVALID_INPUT;

    // now that everything is ok, transfer the buffer to the unit
    unit->output_buffer = output_buffer.release();

    return SRCML_STATUS_OK;
}

/**
 * srcml_write_end_unit
 * @param archive a srcml archive opened for writing
 *
 * End by element output and output the end tag unit to the srcml_archive archive.
 * srcml_write_start_unit must be called first.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_end_unit(struct srcml_unit* unit) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->unit_translator == nullptr || !unit->unit_translator->add_end_unit())
        return SRCML_STATUS_INVALID_INPUT;

    // finished with any parsing
    delete unit->unit_translator;
    unit->unit_translator = 0;

    // store the output in a buffer
    // @todo check into xmlBufferDetach()
    unit->srcml.clear();
    unit->srcml.append((const char *)unit->output_buffer->content, unit->output_buffer->use);

    xmlBufferFree(unit->output_buffer);

    return SRCML_STATUS_OK;
}

/**
 * srcml_write_start_element
 * @param archive a srcml archive opened for writing
 * @param prefix the namespace prefix for element
 * @param name the name of the element
 * @param uri the namespace uri for element
 *
 * Start an element and write to the srcml_archive archive.
 * srcml_write_start_unit must be called first.
 * A unit tag may not be written.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_start_element(struct srcml_unit* unit, const char* prefix, const char* name, const char* uri) {

    // prefix can be default
    if (unit == nullptr || name == nullptr || uri == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->unit_translator == nullptr || !unit->unit_translator->add_start_element(prefix, name, uri))
        return SRCML_STATUS_INVALID_INPUT;

    return SRCML_STATUS_OK;
}

/**
 * srcml_write_end_element
 * @param archive a srcml archive opened for writing
 *
 * Output an end tag to the srcml_archive archive.
 * srcml_write_start_unit must be called first.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_end_element(struct srcml_unit* unit) {

    if (unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->unit_translator == nullptr || !unit->unit_translator->add_end_element())
        return SRCML_STATUS_INVALID_INPUT;

    return SRCML_STATUS_OK;
}

/**
 * srcml_write_namespace
 * @param archive a srcml archive opened for writing
 * @param prefix the namespace prefix
 * @param uri the namespace uri
 *
 * Write a namespace on an element.  No checking is done to see if valid place for a namespace.
 * i.e being added to start tag.
 * srcml_write_start_unit must be called first.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */

int srcml_write_namespace(struct srcml_unit* unit, const char* prefix, const char* uri) {

    // prefix allowed to be default (null)
    if (unit == nullptr || uri == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->unit_translator == nullptr || !unit->unit_translator->add_namespace(prefix, uri))
        return SRCML_STATUS_INVALID_INPUT;

    return SRCML_STATUS_OK;
}

/**
 * srcml_write_attribute
 * @param archive a srcml archive opened for writing
 * @param prefix the namespace prefix for attribute
 * @param name the name of the attribute
 * @param uri the namespace uri for attriubute
 * @param content the contents/value of the attribute
 *
 * Write an namespace on an element.  No checking is done to see if valid place for a namespace.
 * i.e being added to start tag.
 * srcml_write_start_unit must be called first.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_attribute(struct srcml_unit* unit, const char* prefix, const char* name, const char* uri, const char* content) {

    if (unit == nullptr || name == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->unit_translator == nullptr || !unit->unit_translator->add_attribute(prefix, name, uri, content))
        return SRCML_STATUS_INVALID_INPUT;

    return SRCML_STATUS_OK;
}

/**
 * srcml_write_string.
 * @param archive a srcml archive opened for writing
 * @param content the string to write out
 *
 * Write the string/text to a started unit.
 * i.e being added to start tag.
 * srcml_write_start_unit must be called first.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_string(struct srcml_unit* unit, const char* content) {

    if (unit == nullptr || content == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (unit->unit_translator == nullptr || !unit->unit_translator->add_string(content))
        return SRCML_STATUS_INVALID_INPUT;

    return SRCML_STATUS_OK;
}

/******************************************************************************
 *                                                                            *
 *                       Unit creation/cleanup functions                      *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_unit_create
 * @param archive a srcml archvie
 *
 * Create a srcml_unit tied to the srcml_archive archive. Internal function.
 *
 * @returns unit on success and on failure returns NULL
 */
struct srcml_unit* srcml_unit_create(struct srcml_archive* archive) {

    if (archive == nullptr)
        return 0;

    srcml_unit* unit;
    try {

        unit = new srcml_unit;

    } catch(...) { return nullptr; }
    
    unit->archive = archive;

    return unit;
}

/** Clone the setup of an existing unit
 * @note Only the setup, not the actual srcML contents, are cloned
 * @note Unit must be freed using srcml_unit_free()
 * @param unit A srcml_unit
 * @return The cloned unit
 */
struct srcml_unit* srcml_unit_clone(const struct srcml_unit* unit) {

    if (unit == nullptr)
        return 0;

    srcml_unit* new_unit = srcml_unit_create(unit->archive);
    new_unit->filename = unit->filename;

    new_unit->hash = unit->hash;
    new_unit->timestamp = unit->timestamp;
    new_unit->version = unit->version;
    new_unit->url = unit->url;
    new_unit->filename = unit->filename;
    new_unit->language = unit->language;
    new_unit->revision = unit->revision;
    new_unit->encoding = unit->encoding;
    new_unit->attributes = unit->attributes;
    new_unit->eol = unit->eol;
    new_unit->derived_language = unit->derived_language;
    new_unit->namespaces = unit->namespaces;

    return new_unit;
}

/**
 * srcml_unit_free
 * @param unit a srcml unit
 *
 * Free the contents of a srcml_unit.
 */
void srcml_unit_free(struct srcml_unit* unit) {

    if (unit == nullptr)
        return;

    if (unit->unit_translator)
        srcml_write_end_unit(unit);

    delete unit;
}

