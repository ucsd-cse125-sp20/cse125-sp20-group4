# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("CSE125" VERSION 0.1.0 LANGUAGES CXX)

# Include sub-projects.
add_subdirectory("Client")
add_subdirectory("Server")
