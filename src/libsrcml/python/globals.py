import os
from ctypes import cdll, c_int, c_char_p
from exception import *

LIBSRCML_PATH=""
if os.path.exists('../../../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../../../bin/libsrcml.dylib"
elif os.path.exists('../../../bin/libsrcml.so') :
    LIBSRCML_PATH="../../../bin/libsrcml.so"
elif os.path.exists('../../../bin/libsrcml.dll') :
    LIBSRCML_PATH="../../../bin/libsrcml.dll"
elif os.path.exists('../bin/libsrcml.dylib') :
    LIBSRCML_PATH="../bin/libsrcml.dylib"
elif os.path.exists('../bin/libsrcml.so') :
    LIBSRCML_PATH="../bin/libsrcml.so"
elif os.path.exists('../bin/libsrcml.dll') :
    LIBSRCML_PATH="../bin/libsrcml.dll"

libsrcml = cdll.LoadLibrary(LIBSRCML_PATH)

# void srcml_cleanup_globals();
libsrcml.srcml_cleanup_globals.restype = None
libsrcml.srcml_cleanup_globals.argtypes = []

# int srcml_version_number();
libsrcml.srcml_version_number.restype = c_int
libsrcml.srcml_version_number.argtypes = []

# const char* srcml_version_string();
libsrcml.srcml_version_string.restype = c_char_p
libsrcml.srcml_version_string.argtypes = []

def cleanup_globals() :
    libsrcml.srcml_cleanup_globals()

def version_number() :
    return libsrcml.srcml_version_number()

def version_string() :
    return libsrcml.srcml_version_string()

# int srcml(const char* input_filename, const char* output_filename);
libsrcml.srcml.restype = c_int
libsrcml.srcml.argtypes = [c_char_p, c_char_p]

# int srcml_set_encoding  (const char* encoding);
libsrcml.srcml_set_encoding.restype = c_int
libsrcml.srcml_set_encoding.argtypes = [c_char_p]

# int srcml_set_language  (const char* language);
libsrcml.srcml_set_language.restype = c_int
libsrcml.srcml_set_language.argtypes = [c_char_p]

# int srcml_set_filename  (const char* filename);
libsrcml.srcml_set_filename.restype = c_int
libsrcml.srcml_set_filename.argtypes = [c_char_p]

# int srcml_set_directory (const char* directory);
libsrcml.srcml_set_directory.restype = c_int
libsrcml.srcml_set_directory.argtypes = [c_char_p]

# int srcml_set_version   (const char* version);
libsrcml.srcml_set_version.restype = c_int
libsrcml.srcml_set_version.argtypes = [c_char_p]

# int srcml_set_all_options   (int option);
libsrcml.srcml_set_all_options.restype = c_int
libsrcml.srcml_set_all_options.argtypes = [c_int]

# int srcml_set_option    (int option);
libsrcml.srcml_set_option.restype = c_int
libsrcml.srcml_set_option.argtypes = [c_int]

# int srcml_clear_option  (int option);
libsrcml.srcml_clear_option.restype = c_int
libsrcml.srcml_clear_option.argtypes = [c_int]

# int srcml_set_tabstop   (int tabstop);
libsrcml.srcml_set_option.restype = c_int
libsrcml.srcml_set_tabstop.argtypes = [c_int]

# int srcml_register_file_extension(const char* extension, const char* language);
libsrcml.srcml_register_file_extension.restype = c_int
libsrcml.srcml_register_file_extension.argtypes = [c_int]

# int srcml_register_namespace(const char* prefix, const char* ns);
libsrcml.srcml_register_namespace.restype = c_int
libsrcml.srcml_register_namespace.argtypes = [c_int]

# const char* srcml_get_encoding ();
libsrcml.srcml_get_encoding.restype = c_char_p
libsrcml.srcml_get_encoding.argtypes = []

# const char* srcml_get_language ();
libsrcml.srcml_get_language.restype = c_char_p
libsrcml.srcml_get_language.argtypes = []

# const char* srcml_get_filename ();
libsrcml.srcml_get_filename.restype = c_char_p
libsrcml.srcml_get_filename.argtypes = []

# const char* srcml_get_directory();
libsrcml.srcml_get_directory.restype = c_char_p
libsrcml.srcml_get_directory.argtypes = []

# const char* srcml_get_version  ();
libsrcml.srcml_get_version.restype = c_char_p
libsrcml.srcml_get_version.argtypes = []

# int         srcml_get_options  ();
libsrcml.srcml_get_options.restype = c_int
libsrcml.srcml_get_options.argtypes = []

# int         srcml_get_tabstop  ();
libsrcml.srcml_get_tabstop.restype = c_int
libsrcml.srcml_get_tabstop.argtypes = []

# int         srcml_get_namespace_size();
libsrcml.srcml_get_namespace_size.restype = c_int
libsrcml.srcml_get_namespace_size.argtypes = []

# const char* srcml_get_prefix(int pos);
libsrcml.srcml_get_prefix.restype = c_char_p
libsrcml.srcml_get_prefix.argtypes = [c_int]

# const char* srcml_get_prefix_uri(const char* namespace_uri);
libsrcml.srcml_get_prefix_uri.restype = c_char_p
libsrcml.srcml_get_prefix_uri.argtypes = [c_char_p]

# const char* srcml_get_namespace(int pos);
libsrcml.srcml_get_namespace.restype = c_char_p
libsrcml.srcml_get_namespace.argtypes = [c_int]

# const char* srcml_get_namespace_prefix(const char* prefix);
libsrcml.srcml_get_namespace_prefix.restype = c_char_p
libsrcml.srcml_get_namespace_prefix.argtypes = [c_char_p]

def srcml(input_filename, output_filename) :
    libsrcml.srcml(input_filename_output_filename)

def set_encoding(self, encoding) :
    check_return(libsrcml.srcml_set_encoding(self.archive, encoding))

def set_language(self, language) :
    check_return(libsrcml.srcml_set_language(self.archive, language))

def set_filename(self, filename) :
    check_return(libsrcml.srcml_set_filename(self.archive, filename))

def set_directory(self, directory) :
    check_return(libsrcml.srcml_set_directory(self.archive, directory))

def set_version(self, version) :
    check_return(libsrcml.srcml_set_version(self.archive, version))

def set_all_options(self, options) :
    check_return(libsrcml.srcml_set_all_options(self.archive, options))

def set_option(self, option) :
    check_return(libsrcml.srcml_set_option(self.archive, option))

def clear_option(self, option) :
    check_return(libsrcml.srcml_clear_option(self.archive, option))

def set_tabstop(self, tabstop) :
    check_return(libsrcml.srcml_set_tabstop(self.archive, tabstop))

def register_file_extension(self, extension, language) :
    check_return(libsrcml.srcml_set_tabstop(self.archive, extension, language))

def register_namespace(self, prefix, ns) :
    check_return(libsrcml.srcml_set_tabstop(self.archive, prefix, ns))

def get_encoding(self) :
    return libsrcml.srcml_get_encoding(self.archive)

def get_language(self) :
    return libsrcml.srcml_get_language(self.archive)

def get_filename(self) :
    return libsrcml.srcml_get_filename(self.archive)

def get_directory(self) :
    return libsrcml.srcml_get_directory(self.archive)

def get_version(self) :
    return libsrcml.srcml_get_version(self.archive)

def get_options(self) :
    return libsrcml.srcml_get_options(self.archive)

def get_tabstop(self) :
    return libsrcml.srcml_get_tabstop(self.archive)
