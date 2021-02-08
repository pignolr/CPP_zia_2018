cmake_minimum_required(VERSION 3.1.0)

project(module_zia DESCRIPTION "compile tous les modules pour le zia")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/php")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ssl")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tester")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/DeflateZipper")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/GZipper")
