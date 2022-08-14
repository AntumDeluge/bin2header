# Common settings for toolchains.

# cross compilers
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++")
if(CMAKE_SYSTEM_NAME EQUAL "Windows")
	set(CMAKE_RC_COMPILER "${TOOLCHAIN_PREFIX}-windres")
endif()

# target environment on build host system
set(CMAKE_FIND_ROOT_PATH "/usr/${TOOLCHAIN_PREFIX}")

# set default behaviour of FIND_XXX() commands:
# - search programs in host environment
# - search headers & libraries in target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM "NEVER")
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY "ONLY")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE "ONLY")

