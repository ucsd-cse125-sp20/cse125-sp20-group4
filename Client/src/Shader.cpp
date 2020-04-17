#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Shaders.h"

constexpr auto SHADER_DIR = "Shaders/";
constexpr auto VERTEX_SHADER_EXT = ".vert";
constexpr auto FRAGMENT_SHADER_EXT = ".frag";

/* Static variables */

Shader Shaders::skyboxShader_ = 0;
Shader Shaders::normalColoringShader_ = 0;
Shader Shaders::curveShader_ = 0;
Shader Shaders::flatShader_ = 0;
Shader Shaders::phongShader_ = 0;
Shader Shaders::horizonShader_ = 0;

/* Private functions */

Shader Shaders::loadShaders( std::string vertex_file, std::string fragment_file ) {

    vertex_file = SHADER_DIR + vertex_file;
    fragment_file = SHADER_DIR + fragment_file;

    std::cout << "Vertex shader: " << vertex_file << std::endl;
    std::cout << "Fragment shader: " << fragment_file << std::endl;

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
        printf( "Impossible to open %s. Check to make sure the file exists and you passed in the right filepath!\n", vertex_file_path );
        printf( "The current working directory is:" );
        // Please for the love of whatever deity/ies you believe in never do something like the next line of code,
        // Especially on non-Windows systems where you can have the system happily execute "rm -rf ~"
    #ifdef _WIN32
        system( "CD" );
    #else
        system( "pwd" );
    #endif
        ( void ) getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream( fragment_file_path, std::ios::in );
    if ( FragmentShaderStream.is_open() ) {
        std::string Line = "";
        while ( getline( FragmentShaderStream, Line ) )
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf( "Compiling shader : %s\n", vertex_file_path );
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
        printf( "%s\n", &VertexShaderErrorMessage[0] );
    } else {
        printf( "Successfully compiled vertex shader!\n" );
    }



    // Compile Fragment Shader
    printf( "Compiling shader : %s\n", fragment_file_path );
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource( FragmentShaderID, 1, &FragmentSourcePointer, NULL );
    glCompileShader( FragmentShaderID );

    // Check Fragment Shader
    glGetShaderiv( FragmentShaderID, GL_COMPILE_STATUS, &Result );
    glGetShaderiv( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
    if ( InfoLogLength > 0 ) {
        std::vector<char> FragmentShaderErrorMessage( InfoLogLength + 1 );
        glGetShaderInfoLog( FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0] );
        printf( "%s\n", &FragmentShaderErrorMessage[0] );
    } else {
        printf( "Successfully compiled fragment shader!\n" );
    }


    // Link the program
    printf( "Linking program\n" );
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
        printf( "%s\n", &ProgramErrorMessage[0] );
    }
    printf( "Successfully Linked Programs\n" );
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

/* Public functions */

void Shaders::initializeShaders() {

    skyboxShader_ = loadShaders( "skyboxShader" );
    normalColoringShader_ = loadShaders( "normalColoringShader" );
    curveShader_ = loadShaders( "curveShader" );
    flatShader_ = loadShaders( "flatShader" );
    phongShader_ = loadShaders( "phongShader" );
    horizonShader_ = loadShaders( "horizonShader.vert", "flatShader.frag" );

}

void Shaders::deleteShaders() {

    glDeleteProgram( skyboxShader_ );
    glDeleteProgram( normalColoringShader_ );
    glDeleteProgram( curveShader_ );
    glDeleteProgram( flatShader_ );
    glDeleteProgram( horizonShader_ );

}

// Getters.
const Shader & Shaders::skyboxShader() { return skyboxShader_; }
const Shader & Shaders::normalColoringShader() { return normalColoringShader_; }
const Shader & Shaders::curveShader() { return curveShader_; }
const Shader & Shaders::flatShader() { return flatShader_; }
const Shader & Shaders::phongShader() { return phongShader_; }
const Shader & Shaders::horizonShader() { return horizonShader_; }