cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0048 NEW)
project(bin2header
	VERSION 0.1.0
	LANGUAGES CXX
)

message("Configuring ${PROJECT_NAME} ${PROJECT_VERSION}")

add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)

# Manpage package/install
add_custom_target(man ALL)
add_custom_command(
	TARGET man
	COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/man/${PROJECT_NAME}.1" "${CMAKE_CURRENT_BINARY_DIR}"
	COMMAND gzip -f9 "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1"
)

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1.gz" DESTINATION man/man1)

# Uninstallation
add_custom_target(uninstall ALL)
add_custom_command(
	TARGET uninstall
	COMMAND rm -vf "${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}"
	COMMAND rm -vf "${CMAKE_INSTALL_PREFIX}/man/man1/${PROJECT_NAME}.1.gz"
)
