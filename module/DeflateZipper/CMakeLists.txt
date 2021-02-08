cmake_minimum_required(VERSION 3.1.0)

project(module_DeflateZipper DESCRIPTION "module de compression deflate pour le zia")
set(LIBRARY_NAME module_DeflateZipper)
set(CMAKE_CXX_STANDARD 17)

set(SRCS
	src/ModuleDeflateZipper.cpp
	)

set(HDRS
	inc/ModuleDeflateZipper.hpp
	)



include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(${LIBRARY_NAME} ${CONAN_LIBS})
