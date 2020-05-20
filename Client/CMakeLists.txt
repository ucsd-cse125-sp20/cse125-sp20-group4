# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Set target executable
add_executable(Client)

# Include header directories
target_include_directories(Client
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Register source files
file(GLOB_RECURSE Client_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(DEBUG "Sources: ${Client_SRC}")
target_sources(Client PRIVATE ${Client_SRC})

# Find dependencies
find_package(spdlog REQUIRED)
find_package(glfw3 CONFIG REQUIRED COMPONENTS GLFW)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Link dependencies

# Shared libraries
target_link_libraries(Client PRIVATE Shared)

# Third-party libraries
target_link_libraries(Client PRIVATE glfw)
target_link_libraries(Client PRIVATE GLEW::GLEW)
target_link_libraries(Client PRIVATE glm)
target_link_libraries(Client PRIVATE assimp::assimp)

# Configure compiler
target_compile_features(Client PRIVATE cxx_std_17)
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

# Models
file(GLOB_RECURSE Client_Models RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Models/*)
if (Client_Models)
    message(STATUS "Including model targets.")
    message(DEBUG "Models: ${Client_Models}")
    list(TRANSFORM Client_Models PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Client_Models_SRC)
    message(TRACE "Model sources: ${Client_Models_SRC}")
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/Models
                       COMMENT "Creating model output directory."
                       )
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Client_Models_SRC} $<TARGET_FILE_DIR:Client>/Models
                       BYPRODUCTS ${Client_Models}
                       COMMENT "Copying models to output."
                       )
else()
    message(STATUS "No model targets found.")
endif()

# Maps
file(GLOB_RECURSE Client_Maps RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Maps/*)
if (Client_Maps)
    message(STATUS "Including map targets.")
    message(DEBUG "Maps: ${Client_Maps}")
    list(TRANSFORM Client_Maps PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Client_Maps_SRC)
    message(TRACE "Map sources: ${Client_Maps_SRC}")
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/Maps
                       COMMENT "Creating map output directory."
                       )
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Client_Maps_SRC} $<TARGET_FILE_DIR:Client>/Maps
                       BYPRODUCTS ${Client_Maps}
                       COMMENT "Copying maps to output."
                       )
else()
    message(STATUS "No map targets found.")
endif()
