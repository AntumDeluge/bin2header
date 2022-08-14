# Toolchain file to cross-compile for 32-bit Linux on 64-bit Posix host.
#
# Usage:
#   *) install cross compiler(s) (Debian/Ubuntu):
#          `sudo apt install g++-multilib-i686-linux-gnu`
#   *) pass toolchain file to CMake:
#          `cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-i686-linux-gnu.cmake <path/to/source/>`

set(CMAKE_SYSTEM_NAME "Linux")
set(TOOLCHAIN_PREFIX "i686-linux-gnu")

include("${CMAKE_CURRENT_LIST_DIR}/toolchain-common.cmake")
