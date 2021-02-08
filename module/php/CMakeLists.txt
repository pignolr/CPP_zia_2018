cmake_minimum_required(VERSION 3.1.0)

project(module_php DESCRIPTION "module PhP pour le zia")
set(LIBRARY_NAME module_php)
set(CMAKE_CXX_STANDARD 17)

set(SRCS
	src/php.cpp
	)

set(HDRS)

set(LIBS
	"-lstdc++fs")

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(${LIBRARY_NAME} ${LIBS})
