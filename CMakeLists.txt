# Version check
cmake_minimum_required (VERSION 3.0)

# Project
project(serialio)

# Add files
file(GLOB sources "*.cpp" "io_context/*.cpp")

# Add library
add_library(serialio STATIC ${sources})

# Linking
if (WIN32)
	find_library(LWS2_32 ws2_32)
	
	target_link_libraries(serialio ${LWS2_32})
endif()

find_package(Boost 1.55 REQUIRED COMPONENTS
	system
)
target_link_libraries(serialio Boost::system)

# Compiler Options
target_compile_options(serialio PRIVATE -fexceptions -std=c++14 -O3 -Wall -pedantic-errors -pedantic)