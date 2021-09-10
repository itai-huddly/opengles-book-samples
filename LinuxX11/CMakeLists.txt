cmake_minimum_required(VERSION 3.10)

# set the project name
project(Examples VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Chapter_2/Hello_Triangle)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup(TARGETS)
endif()
