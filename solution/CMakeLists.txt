cmake_minimum_required(VERSION 3.1.0)

project("zia_solution" DESCRIPTION "Solution Zia")
set(BINARY_NAME zia)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Wunused \
	 -Wpedantic -Wlogical-op")

set(INCLUDE inc)

set(SRCS
	src/main.cpp
	src/dlloader/linux/DynamicLibrary.cpp
	src/dlloader/windows/DynamicLibrary.cpp
	src/server/Server.cpp
	src/dlloader/ModulesManager.cpp
	src/server/Request.cpp
	src/server/ConfigManager.cpp
	src/server/ShellServer.cpp
	src/server/Headers.cpp
	src/default_module/ModuleReader.cpp
	src/default_module/ModuleContextConverter.cpp
	src/default_module/ModuleWriter.cpp
	src/default_module/ModuleDefaultPage.cpp
	src/default_module/ModuleHttpService.cpp
	src/default_module/ModuleHttpRequestParser.cpp
	src/default_module/ModuleHttpResponseParser.cpp
	)

set(HDRS
	inc/zia.hpp
	inc/api/AModulesManager.hpp
	inc/api/Heading.hpp
	inc/api/Stage.hpp
	inc/dlloader/IDynamicLibrary.hpp
	inc/dlloader/linux/DynamicLibrary.hpp
	inc/dlloader/windows/DynamicLibrary.hpp
	inc/dlloader/ModulesManager.hpp
	inc/server/Server.hpp
	inc/server/Request.hpp
	inc/server/ConfigManager.hpp
	inc/server/ShellServer.hpp
	inc/server/Headers.hpp
	inc/default_module/ModuleReader.hpp
	inc/default_module/ModuleContextConverter.hpp
	inc/default_module/ModuleWriter.hpp
	inc/default_module/ModuleDefaultPage.hpp
	inc/default_module/ModuleHttpService.hpp
	inc/default_module/ModuleHttpRequestParser.hpp
	inc/default_module/ModuleHttpResponseParser.hpp
	)

set(LIBS
	"-ldl -lstdc++fs")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

message(STATUS ${SRCS_CLIENT})
message(STATUS ${SRCS_SERVER})

add_executable(${BINARY_NAME} ${SRCS} ${HDRS})
target_include_directories(${BINARY_NAME} PRIVATE ${INCLUDE})
target_link_libraries(${BINARY_NAME} ${CONAN_LIBS} ${LIBS})
