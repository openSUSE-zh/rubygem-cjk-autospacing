require "rbconfig"
if RbConfig::MAKEFILE_CONFIG["CFLAGS"].include?("-g -O2")
  fixed_CFLAGS = RbConfig::MAKEFILE_CONFIG["CFLAGS"].sub("-g -O2", "$(cflags)")
  puts("Fix CFLAGS: #{RbConfig::MAKEFILE_CONFIG["CFLAGS"]} -> #{fixed_CFLAGS}")
  RbConfig::MAKEFILE_CONFIG["CFLAGS"] = fixed_CFLAGS
end

require 'mkmf'

CONFIG["optflags"] = "-O0"
CONFIG["debugflags"] = "-ggdb3"

dir_config("/usr/lib64", "/usr/lib", "/usr/include/unicode")

abort("missing \"unicode/utext.h\"") unless find_header("unicode/utext.h")
find_library("icuuc", "utext_openUTF8", "/usr/lib64", "/usr/lib")
abort("missing \"utext_openUTF8\"") unless have_library("icuuc", "utext_openUTF8", "unicode/utext.h")

create_header
create_makefile 'cjk_auto_space/cjk_auto_space'
