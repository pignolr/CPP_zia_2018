cmake_minimum_required(VERSION 3.1.0)

project(module_tester DESCRIPTION "module tester pour le zia")
set(LIBRARY_NAME module_tester)
set(CMAKE_CXX_STANDARD 17)

set(SRCS
	src/tester.cpp
	)

set(HDRS
	inc/tester.hpp
	)

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
