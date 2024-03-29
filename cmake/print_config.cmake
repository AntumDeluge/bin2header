
set(os_name "unknown")
if(CMAKE_HOST_SYSTEM_NAME)
	set(os_name "${CMAKE_HOST_SYSTEM_NAME}")
	if(CMAKE_CXX_PLATFORM_ID)
		set(os_name "${os_name} (${CMAKE_CXX_PLATFORM_ID})")
	endif()
endif()

set(build_type "Native")
if(NATIVE)
	set(cxx_compiler_id "${CMAKE_CXX_COMPILER_ID}")
	if (CMAKE_CXX_COMPILER_VERSION)
		set(cxx_compiler_id "${cxx_compiler_id} (${CMAKE_CXX_COMPILER_VERSION})")
	endif()
else()
	set(build_type "Python")
endif()


message("\n\n   *** Host System ***\n")
message(STATUS "Name ........... ${os_name}")
message(STATUS "Version ........ ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "Architecture ... ${CMAKE_HOST_SYSTEM_PROCESSOR}")

message("\n\n   *** CMake General ***\n")
message(STATUS "Generator ........ ${CMAKE_GENERATOR}")
#message(STATUS "Build ............ ${CMAKE_BUILD_TYPE}")
message(STATUS "Make program ..... ${CMAKE_MAKE_PROGRAM}")
message(STATUS "Install prefix ... ${CMAKE_INSTALL_PREFIX}")

message("\n\n   *** Target Build ***\n")
if(NATIVE)
	message(STATUS "Build type ........ ${build_type}")
	message(STATUS "C++ compiler ...... ${CMAKE_CXX_COMPILER}")
	message(STATUS "C++ compiler ID ... ${cxx_compiler_id}")
	message(STATUS "C++ standard ...... ${CMAKE_CXX_STANDARD}")
	#message(STATUS "Architecture ...... ")
	message(STATUS "Statically link ... ${STATIC}")
	message(STATUS "Embed icon ........ ${EMBED_ICON} (Windows only)")
else()
	message(STATUS "Build type ... ${build_type}")
endif()

message("\n")
message(STATUS "To see all configured options execute `cmake -LA path/to/source`")

message("\n")
