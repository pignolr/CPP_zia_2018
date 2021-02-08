cmake_minimum_required(VERSION 3.1.0)

project(module_GZipper DESCRIPTION "module de compression gzip pour le zia")
set(LIBRARY_NAME module_GZipper)
set(CMAKE_CXX_STANDARD 17)

set(SRCS
	src/GZipper.cpp
	)

set(HDRS
	inc/GZipper.hpp
	)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(${LIBRARY_NAME} ${CONAN_LIBS})
