# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set target executable
add_executable(Client)

# Include header directories
target_include_directories(Client
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Register source files
file(GLOB_RECURSE Client_SRC CONFIGURE_DEPENDS src/*.cpp)
message(DEBUG "Sources: ${Client_SRC}")
target_sources(Client PRIVATE ${Client_SRC})

# Find dependencies
find_package(spdlog REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(OpenGL REQUIRED COMPONENTS GL      # v3.8
#                                        GLU     # v3.8
#                                        GLX     # v3.10
#                                        EGL     # v3.10
#                                        OpenGL) # v3.10
find_package(glfw3 CONFIG REQUIRED COMPONENTS GLFW)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

# Link dependencies
target_link_libraries(Client PRIVATE spdlog::spdlog)
#target_link_libraries(Client PRIVATE OpenGL::GL)
target_link_libraries(Client PRIVATE glfw)
target_link_libraries(Client PRIVATE GLEW::GLEW)
target_link_libraries(Client PRIVATE glm)

# Configure compiler
target_compile_features(Client PRIVATE cxx_std_11)
if (MSVC)
    target_compile_options(Client PRIVATE /W4 /WX /wd4275 /wd4127)
    target_link_options(Client PRIVATE)
else()
    target_compile_options(Client PRIVATE -Wall -Wextra -Werror)
endif()

# Include resource files in artifacts

# Shaders
file(GLOB_RECURSE Client_Shaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*)
if (Client_Shaders)
    message(STATUS "Including shader targets.")
    message(DEBUG "Shaders: ${Client_Shaders}")
    list(TRANSFORM Client_Shaders PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Client_Shaders_SRC)
    message(TRACE "Shader sources: ${Client_Shaders_SRC}")
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/Shaders
                       COMMENT "Creating shader output directory."
                       )
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Client_Shaders_SRC} $<TARGET_FILE_DIR:Client>/Shaders
                       BYPRODUCTS ${Client_Shaders}
                       COMMENT "Copying shaders to output."
                       )
else()
    message(STATUS "No shader targets found.")
endif()