/**
 * @file libsrcml.cpp
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
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

/*
  Implementation of srcml simple API (global archive) functions from the namespace srcml_*
*/

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_utilities.hpp>

#include <srcMLTranslator.hpp>
#include <Language.hpp>
#include <Options.hpp>
#include <srcmlns.hpp>

#include <string.h>
#include <stdlib.h>
#include <boost/regex.hpp>

#include <vector>
#include <string>
#include <fstream>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

/**
 * @var srcml_error
 *
 * Global structure to hold error string.
 */
std::string srcml_error;

/**
 * @var global_archive
 *
 * global archive for use with srcml() function.  Defaulted values.
 * Archive is used for both read and write first call to srcml()
 * initializes other parameters.
 */
srcml_archive global_archive = { SRCML_ARCHIVE_RW, 0, 0, 0, 0, 0, 0, std::vector<std::string>(),
				 SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL,
                                 8, std::vector<std::string>(), std::vector<std::string>(), std::vector<pair>(),
				 std::vector<std::string>(), 0, 0, 0, 0, std::vector<transform>() };

/******************************************************************************
 *                                                                            *
 *                           Global Cleanup function                          *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_cleanup_globals
 *
 * Cleanup and free globals allocated at the global level
 * usually by libxml2.
 */
__LIBSRCML_DECL void srcml_cleanup_globals() {

  xmlCleanupCharEncodingHandlers();
  xmlCleanupGlobals();
  xmlDictCleanup();
  xmlCleanupParser();

}

/******************************************************************************
 *                                                                            *
 *                           Version functions                                *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_version_string
 *
 * @returns Return version of libsrcml as string.
 */
__LIBSRCML_DECL const char * srcml_version_string() {

  return SRCML_VERSION_STRING;

}

/**
 * srcml_version_number
 * 
 * @returns Return version of libsrcml as number.
 */
__LIBSRCML_DECL int srcml_version_number() {

  return SRCML_VERSION_NUMBER;

}

/******************************************************************************
 *                                                                            *
 *                           srcML Convenience                                *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml
 * @param input_filename input file to turn to srcML or source code.
 * @param output_filename file to output srcML or source code
 * 
 * Translates to/from srcML 
 * Input files with extension xml and language set to xml result in srcml2src behaviour.
 * All other ending extensions result in src2srcml.  Currently, xml files are not detected if they are 
 * in any other archive format e.g. tar, gz i.e. a.xml.tar.gz.  Only a.xml will be detected from extension.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml(const char* input_filename, const char* output_filename) {

  if(!input_filename || !output_filename) {

    srcml_error = "No input file provided";
    return  SRCML_STATUS_ERROR;

  }

  static bool first = true;
  if(first) {

    first = false;
    std::vector<pair> save_ext;
    for(std::vector<pair>::size_type i = 0; i < global_archive.registered_languages.size(); ++i)
      try {
        save_ext.push_back(global_archive.registered_languages.at(i));
      } catch(...) {
        return SRCML_STATUS_ERROR;
      }

    Language::register_standard_file_extensions(global_archive.registered_languages);

    for(std::vector<pair>::size_type i = 0; i < save_ext.size(); ++i)
      try {
        global_archive.registered_languages.push_back(save_ext.at(i));
      } catch(...) {
        return SRCML_STATUS_ERROR;
      }

    std::vector<std::string> save_prefix;
    std::vector<std::string> save_ns;
    try {
      for(std::vector<std::string>::size_type i = 0; i < global_archive.prefixes.size(); ++i) {
        save_prefix.push_back(global_archive.prefixes.at(i));
        save_ns.push_back(global_archive.namespaces.at(i));

      }

    } catch(...) {
      return SRCML_STATUS_ERROR;
    }

    srcml_archive_register_namespace(&global_archive, SRCML_SRC_NS_PREFIX_DEFAULT, SRCML_SRC_NS_URI);
    srcml_archive_register_namespace(&global_archive, SRCML_CPP_NS_PREFIX_DEFAULT, SRCML_CPP_NS_URI);
    srcml_archive_register_namespace(&global_archive, SRCML_ERR_NS_PREFIX_DEFAULT, SRCML_ERR_NS_URI);
    srcml_archive_register_namespace(&global_archive, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT, SRCML_EXT_LITERAL_NS_URI);
    srcml_archive_register_namespace(&global_archive, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT, SRCML_EXT_OPERATOR_NS_URI);
    srcml_archive_register_namespace(&global_archive, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT, SRCML_EXT_MODIFIER_NS_URI);
    srcml_archive_register_namespace(&global_archive, SRCML_EXT_POSITION_NS_PREFIX_DEFAULT, SRCML_EXT_POSITION_NS_URI);

    for(std::vector<std::string>::size_type i = 0; i < save_prefix.size(); ++i) {
      try {
        srcml_archive_register_namespace(&global_archive, save_prefix.at(i).c_str(), save_ns.at(i).c_str());
      } catch(...) {
        return SRCML_STATUS_ERROR;
      }
    }

  }

  int lang = global_archive.language ? srcml_check_language(global_archive.language->c_str()) : Language::getLanguageFromFilename(input_filename, global_archive.registered_languages);

  if(lang) {

    OPTION_TYPE & options = global_archive.options;
    options |= lang == Language::LANGUAGE_JAVA ? 0 : SRCML_OPTION_CPP;

    srcMLTranslator translator(lang,
                               global_archive.encoding ? global_archive.encoding->c_str() : "UTF-8",
                               global_archive.encoding ? global_archive.encoding->c_str() : "UTF-8",
                               output_filename,
                               options,
                               0,
                               0,
                               0,
                               0,
                               global_archive.tabstop,
                               &global_archive.prefixes.front());
    int error = 0;
    try {

      translator.setInput(input_filename);
      translator.translate(global_archive.directory ? global_archive.directory->c_str() : 0,
                           global_archive.filename ? global_archive.filename->c_str() : input_filename,
                           global_archive.version ? global_archive.version->c_str() : 0,
                           lang);
      options &= ~SRCML_OPTION_CPP;

    } catch (FileError) {

      error = 1;
      srcml_error = "Error converting '";
      srcml_error += input_filename;
      srcml_error += "' to srcML.";

    }

    translator.close();

    if(error)
      return  SRCML_STATUS_ERROR;

  } else {

    bool is_xml = false;
    size_t len = strlen(input_filename);
    if((len > 4 && input_filename[len - 1] == 'l' && input_filename[len - 2] == 'm'
        && input_filename[len - 3] == 'x' && input_filename[len - 4] == '.')
       || (global_archive.language && strcmp(global_archive.language->c_str(), "xml") == 0))
      is_xml = true;

    // not xml or handled language
    if(!is_xml) {

      if(global_archive.language) {
        srcml_error = "Language '";
        srcml_error += global_archive.language->c_str();
        srcml_error += "' is not supported.";
      } else
        srcml_error = "No language provided.";

      return SRCML_STATUS_ERROR;

    }

    std::ifstream in(input_filename);
    in.seekg(0,std::ios::end);
    std::streampos length = in.tellg();
    in.seekg(0,std::ios::beg);
    if(length == std::streampos(0)) return SRCML_STATUS_ERROR;

    std::vector<char> buffer(length);
    in.read(&buffer[0], length);

    OPTION_TYPE & options = global_archive.options;

    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(output_filename, xmlFindCharEncodingHandler(global_archive.encoding ? global_archive.encoding->c_str() : "ISO-8859-1"), global_archive.options & SRCML_OPTION_COMPRESS);
    srcml_extract_text(&buffer[0], buffer.size(), output_buffer, options, 0);

  }

  return SRCML_STATUS_OK;
}

/******************************************************************************
 *                                                                            *
 *                           Global set functions                             *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_set_encoding
 * @param encoding an output encoding
 *
 * Set the output encoding.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_encoding(const char* encoding) {

  return srcml_archive_set_encoding(&global_archive, encoding);

}

/**
 * srcml_set_language
 * @param language a language
 *
 * Set the language to use to parse.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_language(const char* language) {

  return srcml_archive_set_language(&global_archive, language);

}

/**
 * srcml_set_filename
 * @param filename name of a file
 *
 * Set the filename attribute for the root unit.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_filename(const char* filename) {

  return srcml_archive_set_filename(&global_archive, filename);

}

/**
 * srcml_set_directory
 * @param directory a directory path
 *
 * Set the directory attribute for the root unit.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_directory(const char* directory) {

  return srcml_archive_set_directory(&global_archive, directory);

}

/**
 * srcml_set_version
 * @param version a version string
 *
 * Set the version attribute.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_version(const char* version) {

  return srcml_archive_set_version(&global_archive, version);

}

/**
 * srcml_set_options
 * @param option a srcml options
 *
 * Set the srcml options.  Clears all previously set.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_options(unsigned long long option) {

  return srcml_archive_set_options(&global_archive, option);

}

/**
 * srcml_enable_option
 * @param option a srcml option
 *
 * Enable/set the srcml options.  Multiple may be enabled.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_enable_option(unsigned long long option) {

  return srcml_archive_enable_option(&global_archive, option);

}

/**
 * srcml_disable_option
 * @param option a srcml option
 *
 * Remove an option.  May use multiple option with the same call.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_disable_option(unsigned long long option) {

  return srcml_archive_disable_option(&global_archive, option);

}

/**
 * srcml_set_tabstop
 * @param tabstop tabstop size
 *
 * Set the size of the tabstop.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_set_tabstop(int tabstop) {

  return srcml_archive_set_tabstop(&global_archive, tabstop);

}

/**
 * srcml_register_file_extension
 * @param extension a file extension
 * @param language a language
 *
 * Associate the given extension with the given language.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_register_file_extension(const char* extension, const char* language) {

  return srcml_archive_register_file_extension(&global_archive, extension, language);

}

/**
 * srcml_register_namespace
 * @param prefix a XML namespace prefix
 * @param ns a XML namespace
 *
 * Add a new namespace or change the prefix of an existing namespace.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_register_namespace(const char* prefix, const char* ns) {

  return srcml_archive_register_namespace(&global_archive, prefix, ns);

}

/**
 * srcml_archive_register_macro
 * @param archive a srcml_archive
 * @param token name of macro
 * @param type macro type
 *
 * Register a macro (token) to be processed as a special type
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
__LIBSRCML_DECL int srcml_register_macro(const char* prefix, const char* ns) {

  return srcml_archive_register_macro(&global_archive, prefix, ns);

}

/******************************************************************************
 *                                                                            *
 *                           Global get functions                             *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_get_encoding
 *
 * @returns Get the output encoding on success and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_encoding() {

  return srcml_archive_get_encoding(&global_archive);

}

/**
 * srcml_get_language
 *
 * @returns Get the language on success and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_language() {

  return srcml_archive_get_language(&global_archive);

}

/**
 * srcml_get_filename
 *
 * @returns Get the filename attribute for the root unit on success
 * and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_filename() {

  return srcml_archive_get_filename(&global_archive);

}

/**
 * srcml_get_directory
 *
 * @returns Get the directory attribute for the root unit on success
 * and NULL on failure
 */
__LIBSRCML_DECL const char* srcml_get_directory() {

  return srcml_archive_get_directory(&global_archive);

}

/**
 * srcml_get_version
 *
 * @returns Get the version attribute on success and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_version() {

  return srcml_archive_get_version(&global_archive);

}

/**
 * srcml_get_options
 *
 * @returns Get the currently set options on success and NULL on failure.
 */
__LIBSRCML_DECL unsigned long long srcml_get_options() {

  return srcml_archive_get_options(&global_archive);

}

/**
 * srcml_get_tabstop
 *
 * @returns Get the tabstop size on success and NULL On failure.
 */
__LIBSRCML_DECL int srcml_get_tabstop() {

  return srcml_archive_get_tabstop(&global_archive);

}

/**
 * srcml_get_namespace_size
 *
 * @returns Get the number of currently defined namespaces.
 */
__LIBSRCML_DECL int srcml_get_namespace_size() {

  return srcml_archive_get_namespace_size(&global_archive);

}

/**
 * srcml_get_prefix
 * @param pos namespace position
 *
 * @returns Get prefix for the given position on success
 * and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_prefix(int pos) {

  return srcml_archive_get_prefix(&global_archive, pos);

}

/**
 * srcml_get_prefix_uri
 * @param namespace_uri an XML namespace
 *
 * @returns Get the registered prefix for the given namespace
 * on success and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_prefix_uri(const char* namespace_uri) {

  return srcml_archive_get_prefix_uri(&global_archive, namespace_uri);

}

/**
 * srcml_get_namespace
 * @param pos position in namespaces
 *
 * @returns Get the namespace at the given pos on succcess
 * and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_namespace(int pos) {

  return srcml_archive_get_namespace(&global_archive, pos);

}

/**
 * srcml_get_namespace_prefix
 * @param prefix an XML prefix
 *
 * @returns Get the first namespace for the given prefix on success
 * and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_namespace_prefix(const char* prefix) {

  return srcml_archive_get_namespace_prefix(&global_archive, prefix);

}

/**
 * srcml_archive_get_macro_list_size
 *
 * @returns Get the number of currently defined macros.
 */
__LIBSRCML_DECL int srcml_get_macro_list_size() {

  return srcml_archive_get_macro_list_size(&global_archive);

}

/**
 * srcml_archive_get_macro_token
 * @param pos macro position
 *
 * @returns Get token for the given position on success
 * and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_macro_token(int pos) {

  return srcml_archive_get_macro_token(&global_archive, pos);

}

/**
 * srcml_archive_get_macro_token_type
 * @param token a macro token
 *
 * @returns Get the registered type for the given token
 * on success and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_macro_token_type(const char* token) {

  return srcml_archive_get_macro_token_type(&global_archive, token
);

}

/**
 * srcml_archive_get_macro_type
 * @param pos position in macro list
 *
 * @returns Get the type at the given pos on succcess
 * and NULL on failure.
 */
__LIBSRCML_DECL const char* srcml_get_macro_type(int pos) {

  return srcml_archive_get_macro_type(&global_archive, pos);

}

/******************************************************************************
 *                                                                            *
 *                           libsrcml utility functions                       *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_check_language
 * @param language a language
 *
 * Check if the current language is supported.
 * @returns Return the numeric representation for that language if supported.
 * Not supported returns 0.
 */
__LIBSRCML_DECL int srcml_check_language(const char* language) { return language == 0 ? 0 : Language::getLanguage(language); }

/**
 * srcml_language_list
 *
 * Gets list of supported source-code languages as NULL-terminated array.
 *
 * @returns NULL-terminated array of supported source-code languages 
*/
__LIBSRCML_DECL const char** srcml_language_list() {
  static const char* langs[] = { "C", "C++", "C#", "Java", 0 };
  return langs;
}

/**
 * srcml_check_extension
 * @param filename name of a file
 *
 * Get the currently registered language for a file extension
 * Full filename can be provided, and extension will be extracted.
 * @returns Returns language on success and NULL on failure. 
 */
__LIBSRCML_DECL const char * srcml_check_extension(const char* filename) {

  return srcml_archive_check_extension(&global_archive, filename);

}

/**
 * srcml_check_format
 * @param format an archive or compression extension, e.g., tar.gz
 *
 * Check if the format is currently supported
 * Full filename can be provided, and extension will be extracted 
 * @returns Return if format is supported.
 */
__LIBSRCML_DECL int srcml_check_format(const char* format) {

  if(format == NULL) return 0;

  static const boost::regex extRegEx("\\.(xz|zg|bz2|tar).*$");

  // reversed copy of the path
  std::size_t length = strlen(format);
  boost::cmatch what;

  if(boost::regex_search(format, format + length, what,extRegEx)) {

    return 1;

  } else
    return 0;

}

/**
 * srcml_check_encoding
 * @param encoding an encoding
 *
 * Check if the particular encoding is supported, both for input and output.
 * @returns Return if encoding is supported.
 */
__LIBSRCML_DECL int srcml_check_encoding(const char* encoding) {

  return xmlParseCharEncoding(encoding) > 0;

}

/**
 * srcml_check_xslt
 * 
 * Check whether xslt is available.
 * @returns Return 1 on success and 0 on failure.
 */ 
__LIBSRCML_DECL int srcml_check_xslt() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
  void * handle = dlopen("libxslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libxslt.so.1", RTLD_LAZY);
    if (!handle) {
      handle = dlopen("libxslt.dylib", RTLD_LAZY);
      if (!handle) return 0;

    }
  }

  dlclose(handle);
  return 1;
#else
  return 1;
#endif

}

/**
 * srcml_check_exslt
 * 
 * Check whether exslt is available.
 * @returns Return 1 on success and 0 on failure.
 */
__LIBSRCML_DECL int srcml_check_exslt() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
  void* handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libexslt.so.0", RTLD_LAZY);
    if (!handle) {
      handle = dlopen("libexslt.dylib", RTLD_LAZY);
      if (!handle) return 0;
    }
  }

  dlclose(handle);
  return 1;
#else
  return 1;
#endif
}

/******************************************************************************
 *                                                                            *
 *                           libsrcml error functions                         *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_error_string
 * 
 * @returns Return a string describing last recorded error.
 */
__LIBSRCML_DECL const char* srcml_error_string() { return srcml_error.c_str(); }
