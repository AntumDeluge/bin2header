cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0048 NEW)
project(bin2header
	VERSION 0.1.3
	LANGUAGES CXX
)
set(PROJECT_PY_VERSION 0.1.2)

message("Configuring ${PROJECT_NAME} ${PROJECT_VERSION}")

if(WIN32)
	set(EMBED_ICON OFF CACHE BOOL "Embeds icon resource into executable on Win32")
endif()

# retrieve source files
file(GLOB FILES_C "${CMAKE_SOURCE_DIR}/src/*.cpp")
if(WIN32 AND EMBED_ICON)
	set(FILES_C ${FILES_C} "${CMAKE_SOURCE_DIR}/icon_resource.rc")
endif()
include_directories("${CMAKE_SOURCE_DIR}/src/include")
add_executable(${PROJECT_NAME} ${FILES_C})

if(WIN32)
	# static link to libgcc & libstdc++
	set(CMAKE_CXX_FLAGS "-static")
endif()

# Manpage package/install
add_custom_target(man ALL)
add_custom_command(
	TARGET man
	COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/man/${PROJECT_NAME}.1" "${CMAKE_CURRENT_BINARY_DIR}"
	COMMAND gzip -f9 "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1"
)

set(DOCDIR "share/doc/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1.gz" DESTINATION share/man/man1)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt" DESTINATION ${DOCDIR})

# Uninstallation
add_custom_target(uninstall)
add_custom_command(
	TARGET uninstall
	COMMAND rm -vf "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}"
	COMMAND rm -vf "${CMAKE_INSTALL_PREFIX}/share/man/man1/${PROJECT_NAME}.1.gz"
	COMMAND rm -vf "${CMAKE_INSTALL_PREFIX}/${DOCDIR}/LICENSE.txt"
)

set_property(
	DIRECTORY
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.1.gz
)
