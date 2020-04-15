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
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(OpenGL REQUIRED COMPONENTS GL      # v3.8
#                                        GLU     # v3.8
#                                        GLX     # v3.10
#                                        EGL     # v3.10
#                                        OpenGL) # v3.10

# Link dependencies
target_link_libraries(Client PRIVATE spdlog::spdlog)
target_link_libraries(Client PRIVATE OpenGL::GL)
target_link_libraries(Client PRIVATE GLUT::GLUT)

# Configure compiler
target_compile_features(Client PRIVATE cxx_std_11)
if (MSVC)
    target_compile_options(Client PRIVATE /W4 /WX /wd4275 /wd4505)
else()
    target_compile_options(Client PRIVATE -Wall -Wextra -Werror)
endif()