# CMakeList.txt : CMake project for Shared, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Set target library
add_library(Shared)

# Include header directories
target_include_directories(Shared
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Register source files
file(GLOB_RECURSE Shared_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(DEBUG "Sources: ${Shared_SRC}")
target_sources(Shared PRIVATE ${Shared_SRC})

# Find dependencies
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Link dependencies
target_link_libraries(Shared PUBLIC spdlog::spdlog_header_only)
target_link_libraries(Shared PUBLIC glm)


# Configure compiler
target_compile_features(Shared PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(Shared PRIVATE /W4 /WX- /wd4275)
    target_link_options(Shared PRIVATE)

else()
    target_compile_options(Shared PRIVATE -Wall -Wextra -Werror)
endif()