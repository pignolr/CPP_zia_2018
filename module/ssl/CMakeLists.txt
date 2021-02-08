cmake_minimum_required(VERSION 3.1.0)

project(module_ssl DESCRIPTION "module ssl pour le zia")
set(LIBRARY_NAME module_ssl)
set(CMAKE_CXX_STANDARD 17)

set(SRCS
	src/ssl.cpp
	src/SSLConnection.cpp)

set(HDRS
	inc/SSLConnection.hpp)

set(LIBS
	"-L/usr/lib -lssl -lcrypto")

add_library(${LIBRARY_NAME} SHARED ${SRCS} ${HDRS})
target_link_libraries(${LIBRARY_NAME} ${LIBS})
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
