# Toolchain file for cross-compiling 32-bit on 64-bit Debian/Ubuntu host system.
#
# Usage:
#   *) install cross compilers (Debian/Ubuntu): `sudo apt install gcc-multilib g++-multilib`
#   *) cmake -DCMAKE_TOOLCHAIN_FILE=<path/to/source/>cmake/gcc-i686.cmake <path/to/source/>

set(CMAKE_SYSTEM_NAME "Linux")
set(TOOLCHAIN_PREFIX "i686-linux-gnu")

# C/C++ cross compilers
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++")

# target environment on build host system
set(CMAKE_FIND_ROOT_PATH "/usr/${TOOLCHAIN_PREFIX}")

# change default behaviour of FIND_XXX() commands:
# - search programs in host environment
# - search headers & libraries in target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM "NEVER")
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY "ONLY")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE "ONLY")
