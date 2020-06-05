# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

message(STATUS "Mode: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in debugging mode.")
    set(BUILD_DEBUG true)
else()
    message(STATUS "Building in release mode.")
endif()

# Set target executable
add_executable(Client)

# Include header directories
target_include_directories(Client
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (DEFINED ENV{FMOD_PATH})
    string(REGEX REPLACE "\\\\" "/" FMOD_PATH $ENV{FMOD_PATH})
    message(STATUS "FMOD path: ${FMOD_PATH}")
else()
    message(SEND_ERROR "FMOD path not specified.")
endif()

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
    target_compile_options(Client PRIVATE /W4 /WX- /wd4275 /wd4127)
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

# Textures
file(GLOB_RECURSE Client_Textures RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Textures/*)
if (Client_Textures)
    message(STATUS "Including textures targets.")
    message(DEBUG "Textures: ${Client_Textures}")
    list(TRANSFORM Client_Textures PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Client_Textures_SRC)
    message(TRACE "Textures sources: ${Client_Textures_SRC}")
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/Textures
                       COMMENT "Creating textures output directory."
                       )
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Client_Textures_SRC} $<TARGET_FILE_DIR:Client>/Textures
                       BYPRODUCTS ${Client_Textures}
                       COMMENT "Copying textures to output."
                       )
else()
    message(STATUS "No texture targets found.")
endif()


if (DEFINED FMOD_PATH)
    # Sound Banks
    file(GLOB_RECURSE Client_Sounds RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../FMOD/SoundEffects/Build/Desktop 
         CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../FMOD/SoundEffects/Build/Desktop/*)
    if (Client_Sounds)
        message(STATUS "Including sound bank targets.")
        message(DEBUG "Sound banks: ${Client_Sounds}")
        list(TRANSFORM Client_Sounds PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/../FMOD/SoundEffects/Build/Desktop/ OUTPUT_VARIABLE Client_Sounds_SRC)
        message(TRACE "Sound bank sources: ${Client_Sounds_SRC}")
        add_custom_command(TARGET Client POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/Sounds
                           COMMENT "Creating sound bank output directory."
                           )
        add_custom_command(TARGET Client POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                                ${Client_Sounds_SRC} $<TARGET_FILE_DIR:Client>/Sounds
                           BYPRODUCTS ${Client_Sounds}
                           COMMENT "Copying sound banks to output."
                           )
    else()
        message(STATUS "No sound bank targets found.")
    endif()
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
    set(TARGET_PLATFORM "x64")
else()
    set(TARGET_PLATFORM "unknown")
endif()

message(DEBUG "Target platform: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Target platform: ${TARGET_PLATFORM}")

if (DEFINED FMOD_PATH AND NOT ${TARGET_PLATFORM} STREQUAL "unknown")
    # FMOD core
    if (BUILD_DEBUG)
        set(FMOD_CORE_LIB "fmodL_vc.lib")
        set(FMOD_CORE_DLL "fmodL.dll")
    else()
        set(FMOD_CORE_LIB "fmod_vc.lib")
        set(FMOD_CORE_DLL "fmod.dll")
    endif()

    add_library(fmod STATIC IMPORTED)
    set_target_properties(fmod PROPERTIES
        IMPORTED_LOCATION "${FMOD_PATH}/api/core/lib/${TARGET_PLATFORM}/${FMOD_CORE_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${FMOD_PATH}/api/core/inc/"
    )
    target_link_libraries(Client PRIVATE fmod)
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            "${FMOD_PATH}/api/core/lib/${TARGET_PLATFORM}/${FMOD_CORE_DLL}" $<TARGET_FILE_DIR:Client>
                       BYPRODUCTS "${FMOD_CORE_DLL}"
                       COMMENT "Copying FMOD core DLLs to output."
                       )

    # FMOD studio
    if (BUILD_DEBUG)
        set(FMOD_STUDIO_LIB "fmodstudioL_vc.lib")
        set(FMOD_STUDIO_DLL "fmodstudioL.dll")
    else()
        set(FMOD_STUDIO_LIB "fmodstudio_vc.lib")
        set(FMOD_STUDIO_DLL "fmodstudio.dll")
    endif()

    add_library(fmod_studio STATIC IMPORTED)
    set_target_properties(fmod_studio PROPERTIES
        IMPORTED_LOCATION "${FMOD_PATH}/api/studio/lib/${TARGET_PLATFORM}/${FMOD_STUDIO_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${FMOD_PATH}/api/studio/inc/"
    )
    target_link_libraries(Client PRIVATE fmod_studio)
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            "${FMOD_PATH}/api/studio/lib/${TARGET_PLATFORM}/${FMOD_STUDIO_DLL}" $<TARGET_FILE_DIR:Client>
                       BYPRODUCTS "${FMOD_STUDIO_DLL}"
                       COMMENT "Copying FMOD studio DLLs to output."
                       )
endif()

file(GLOB_RECURSE Client_Fonts RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui/fonts/*.ttf)
if (Client_Fonts)
    message(STATUS "Including model targets.")
    message(DEBUG "Fonts: ${Client_Fonts}")
    list(TRANSFORM Client_Fonts PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Client_Fonts_SRC)
    message(TRACE "Fonts sources: ${Client_Fonts_SRC}")
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/fonts
                       COMMENT "Creating font output directory."
                       )
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Client_Fonts_SRC} $<TARGET_FILE_DIR:Client>/fonts
                       BYPRODUCTS ${Client_Fonts}
                       COMMENT "Copying fonts to output."
                       )
else()
    message(STATUS "No fonts targets found.")
endif()

# config
file(GLOB_RECURSE Client_Config RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Config/*)
if (Client_Config)
    message(STATUS "Including map targets.")
    message(DEBUG "Config: ${Client_Config}")
    list(TRANSFORM Client_Config PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Client_Config_SRC)
    message(TRACE "Config sources: ${Client_Config_SRC}")
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/Config
                       COMMENT "Creating config output directory."
                       )
    add_custom_command(TARGET Client POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Client_Config_SRC} $<TARGET_FILE_DIR:Client>/Config
                       BYPRODUCTS ${Client_Maps}
                       COMMENT "Copying config to output."
                       )
else()
    message(STATUS "No config targets found.")
endif()