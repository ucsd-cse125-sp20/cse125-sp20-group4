# CMakeList.txt : CMake project for Server, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Set target executable
add_executable(Server)

# Include source directories
target_include_directories(Server
    SYSTEM PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include

)
# Register source files
file(GLOB_RECURSE Server_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(DEBUG "Sources: ${Server_SRC}")
target_sources(Server PRIVATE ${Server_SRC})


# Find dependencies
find_package(spdlog REQUIRED)
find_package(glm CONFIG REQUIRED)
# Link dependencies
#target_link_libraries(Server PUBLIC spdlog::spdlog)

# Link dependencies
target_link_libraries(Server PRIVATE Shared)
target_link_libraries(Server PRIVATE glm)

# Configure compiler
target_compile_features(Server PRIVATE cxx_std_17)
if (MSVC)
    target_compile_options(Server PRIVATE /W4 /WX /wd4275)
else()
    target_compile_options(Server PRIVATE -Wall -Wextra -Werror)
endif()