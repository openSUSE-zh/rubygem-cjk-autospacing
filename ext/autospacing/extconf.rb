require 'mkmf'

dir_config("/usr/lib64", "/usr/lib", "/usr/include/unicode")

abort("missing \"unicode/utext.h\"") unless find_header("unicode/utext.h")
abort("missing \"unicode/ustdio.h\"") unless find_header("unicode/ustdio.h")
find_library("icuuc", "utext_openUTF8", "/usr/lib64", "/usr/lib")
abort("missing \"utext_openUTF8\"") unless have_library("icuuc", "utext_openUTF8", "unicode/utext.h")

create_header
create_makefile 'autospacing/autospacing'
