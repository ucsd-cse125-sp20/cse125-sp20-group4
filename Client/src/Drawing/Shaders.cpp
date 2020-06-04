#include <stdio.h>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>

#include "logger.h"

#include "drawing/Shaders.h"

static const auto LOGGER = getLogger( "Shaders" );

static constexpr auto SHADER_DIR = "Shaders/";
static constexpr auto VERTEX_SHADER_EXT = ".vert";
static constexpr auto FRAGMENT_SHADER_EXT = ".frag";

/* Private variables */

std::unordered_map<std::string, Shader> Shaders::shaders;

/* Public functions */

void Shaders::initializeShaders() {

    shaders["skybox"] = loadShaders( "skyboxShader" );
    shaders["normal_coloring"] = loadShaders( "normalColoringShader" );
    shaders["curve"] = loadShaders( "curveShader" );
    shaders["flat"] = loadShaders( "flatShader" );
    shaders["phong"] = loadShaders( "phongShader" );
    shaders["horizon"] = loadShaders( "horizonShader.vert", "flatShader.frag" );
    shaders["particle"] = loadShaders( "particleShader" );

}

void Shaders::deleteShaders() {

    for ( auto it = shaders.cbegin(); it != shaders.cend(); it++ ) {
        glDeleteProgram( it->second );
    }

}

// Getters.
const Shader & Shaders::get( const std::string shader ) {

    try {
        return shaders.at( shader );
    } catch ( const std::out_of_range & ) {
        LOGGER->critical( "Invalid shader: {}", shader );
        throw std::invalid_argument( "Invalid shader: " + shader );
    }

}

const Shader & Shaders::skybox() { return get( "skybox" ); }
const Shader & Shaders::normalColoring() { return get( "normal_coloring" ); }
const Shader & Shaders::curve() { return get( "curve" ); }
const Shader & Shaders::flat() { return get( "flat" ); }
const Shader & Shaders::phong() { return get( "phong" ); }
const Shader & Shaders::horizon() { return get( "horizon" ); }
const Shader& Shaders::particle() { return get("particle");  }

/* Private functions */

Shader Shaders::loadShaders( std::string vertex_file, std::string fragment_file ) {

    vertex_file = SHADER_DIR + vertex_file;
    fragment_file = SHADER_DIR + fragment_file;

    LOGGER->trace( "Vertex shader: {}", vertex_file );
    LOGGER->trace( "Fragment shader: {}", fragment_file );

    const char * vertex_file_path = vertex_file.c_str();
    const char * fragment_file_path = fragment_file.c_str();

    // Create the shaders
    GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
    GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream( vertex_file_path, std::ios::in );
    if ( VertexShaderStream.is_open() ) {
        std::string Line = "";
        while ( getline( VertexShaderStream, Line ) )
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    } else {
        LOGGER->critical( "Impossible to open vertex shader {}. Check to make sure the file exists and you passed in the right filepath!", vertex_file );
        printf( "The current working directory is:" );
        // Please for the love of whatever deity/ies you believe in never do something like the next line of code,
        // Especially on non-Windows systems where you can have the system happily execute "rm -rf ~"
    #ifdef _WIN32
        system( "CD" );
    #else
        system( "pwd" );
    #endif
        ( void ) getchar();
        throw std::invalid_argument( "Invalid vertex shader path: " + vertex_file );
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream( fragment_file_path, std::ios::in );
    if ( FragmentShaderStream.is_open() ) {
        std::string Line = "";
        while ( getline( FragmentShaderStream, Line ) )
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    } else {
        LOGGER->critical( "Impossible to open fragment shader {}. Check to make sure the file exists and you passed in the right filepath!", fragment_file );
        printf( "The current working directory is:" );
        // Please for the love of whatever deity/ies you believe in never do something like the next line of code,
        // Especially on non-Windows systems where you can have the system happily execute "rm -rf ~"
    #ifdef _WIN32
        system( "CD" );
    #else
        system( "pwd" );
    #endif
        ( void ) getchar();
        throw std::invalid_argument( "Invalid fragment shader path: " + fragment_file );
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    LOGGER->debug( "Compiling vertex shader : {}", vertex_file );
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource( VertexShaderID, 1, &VertexSourcePointer, NULL );
    glCompileShader( VertexShaderID );

#pragma warning( push )
#pragma warning( disable: 26451 )

    // Check Vertex Shader
    glGetShaderiv( VertexShaderID, GL_COMPILE_STATUS, &Result );
    glGetShaderiv( VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
    if ( InfoLogLength > 0 ) {
        std::vector<char> VertexShaderErrorMessage( InfoLogLength + 1 );
        glGetShaderInfoLog( VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0] );
        LOGGER->critical( "Encountered error while compiling vertex shader: {}", &VertexShaderErrorMessage[0] );
        throw std::invalid_argument( "Error compiling vertex shader: " + vertex_file );
    } else {
        LOGGER->debug( "Successfully compiled vertex shader!" );
    }



    // Compile Fragment Shader
    LOGGER->debug( "Compiling fragment shader: {}", fragment_file );
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource( FragmentShaderID, 1, &FragmentSourcePointer, NULL );
    glCompileShader( FragmentShaderID );

    // Check Fragment Shader
    glGetShaderiv( FragmentShaderID, GL_COMPILE_STATUS, &Result );
    glGetShaderiv( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
    if ( InfoLogLength > 0 ) {
        std::vector<char> FragmentShaderErrorMessage( InfoLogLength + 1 );
        glGetShaderInfoLog( FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0] );
        LOGGER->critical( "Encountered error while compiling fragment shader: {}", &FragmentShaderErrorMessage[0] );
        
    } else {
        LOGGER->debug( "Successfully compiled fragment shader!" );
    }


    // Link the program
    LOGGER->debug( "Linking shader program" );
    GLuint ProgramID = glCreateProgram();
    glAttachShader( ProgramID, VertexShaderID );
    glAttachShader( ProgramID, FragmentShaderID );
    glLinkProgram( ProgramID );

    // Check the program
    glGetProgramiv( ProgramID, GL_LINK_STATUS, &Result );
    glGetProgramiv( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
    if ( InfoLogLength > 0 ) {
        std::vector<char> ProgramErrorMessage( InfoLogLength + 1 );
        glGetProgramInfoLog( ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0] );
        LOGGER->critical( "Error while linking shader program: {}", &ProgramErrorMessage[0] );
        throw std::invalid_argument( "Error linking shader program" );
    }
    LOGGER->debug( "Successfully linked shader program" );
    glDetachShader( ProgramID, VertexShaderID );
    glDetachShader( ProgramID, FragmentShaderID );

    glDeleteShader( VertexShaderID );
    glDeleteShader( FragmentShaderID );

#pragma warning( pop )

    return ProgramID;
}

Shader Shaders::loadShaders( std::string shaderName ) {

    return loadShaders( shaderName + VERTEX_SHADER_EXT, shaderName + FRAGMENT_SHADER_EXT );

}