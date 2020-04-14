# CMakeList.txt : CMake project for Server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories("../include")

# Add source to this project's executable.
add_executable (Server "Server.cpp" "Server.h" "logger.h" "logger.cpp")

# TODO: Add tests and install targets if needed.
