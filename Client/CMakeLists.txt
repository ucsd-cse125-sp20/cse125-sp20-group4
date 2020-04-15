# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set target executable
add_executable (Client "src/Client.cpp")

# Include source directories
target_include_directories(Client
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find dependencies
find_package(spdlog REQUIRED)

# Link dependencies
target_link_libraries(Client PRIVATE spdlog::spdlog)

# Configure compiler
target_compile_features(Client PRIVATE cxx_std_11)
if (MSVC)
    target_compile_options(Client PRIVATE /W4 /WX /wd4275)
else()
    target_compile_options(Client PRIVATE -Wall -Wextra -Werror)
endif()