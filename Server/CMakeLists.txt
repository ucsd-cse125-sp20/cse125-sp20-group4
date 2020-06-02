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


file(GLOB_RECURSE Server_Maps RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Maps/*)
if (Server_Maps)
    message(STATUS "Including maps targets.")
    message(DEBUG "Maps: ${Server_Maps}")
    list(TRANSFORM Server_Maps PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Server_Maps_SRC)
    message(TRACE "Maps sources: ${Server_Maps_SRC}")
    add_custom_command(TARGET Server POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Server>/Maps
                       COMMENT "Creating map output directory."
                       )
    add_custom_command(TARGET Server POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Server_Maps_SRC} $<TARGET_FILE_DIR:Server>/Maps
                       BYPRODUCTS ${Server_Maps}
                       COMMENT "Copying maps to output."
                       )
else()
    message(STATUS "No maps targets found.")
endif()


# config
file(GLOB_RECURSE Server_Config RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
     CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Config/*)
if (Server_Config)
    message(STATUS "Including config targets.")
    message(DEBUG "Config: ${Server_Config}")
    list(TRANSFORM Server_Config PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/ OUTPUT_VARIABLE Server_Config_SRC)
    message(TRACE "Config sources: ${Server_Config_SRC}")
    add_custom_command(TARGET Server POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Server>/Config
                       COMMENT "Creating config output directory."
                       )
    add_custom_command(TARGET Server POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                            ${Server_Config_SRC} $<TARGET_FILE_DIR:Server>/Config
                       BYPRODUCTS ${Server_Maps}
                       COMMENT "Copying config to output."
                       )
else()
    message(STATUS "No config targets found.")
endif()

# Configure compiler
target_compile_features(Server PRIVATE cxx_std_17)
if (MSVC)
    target_compile_options(Server PRIVATE /W4 /WX- /wd4275)

else()
    target_compile_options(Server PRIVATE -Wall -Wextra -Werror)
endif()
