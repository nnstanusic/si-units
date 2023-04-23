cmake_minimum_required(VERSION 3.25)

project(SI_UNITS)

add_library(SI INTERFACE)

target_sources(SI INTERFACE 
	include/si.h
	include/si_format.h
	)
target_include_directories(SI INTERFACE include)
enable_testing()
add_subdirectory(tests)