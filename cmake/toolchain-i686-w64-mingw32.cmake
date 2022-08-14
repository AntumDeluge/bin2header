# Toolchain file to cross-compile for 32-bit Windows on 64-bit Posix host.
#
# Usage:
#   *) install cross compiler(s) (Debian/Ubuntu):
#          `sudo apt install mingw-w64`
#   *) pass toolchain file to CMake:
#          `cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-i686-w64-mingw32.cmake <path/to/source/>`

set(CMAKE_SYSTEM_NAME "Windows")
set(TOOLCHAIN_PREFIX "i686-w64-mingw32")

include("${CMAKE_CURRENT_LIST_DIR}/toolchain-common.cmake")
