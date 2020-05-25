#ifndef SHADERS_H
#define SHADERS_H

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

typedef GLuint Shader;

class Shaders {

    private:
    static std::unordered_map<std::string, Shader> shaders;

    static Shader loadShaders( std::string vertex_file, std::string fragment_file );
    static Shader loadShaders( std::string shaderName );

    public:
    static void initializeShaders();
    static void deleteShaders();

    /* Constant getters */
    static const Shader & get( const std::string shader );
    static const Shader & skybox();
    static const Shader & normalColoring();
    static const Shader & curve();
    static const Shader & flat();
    static const Shader & phong();
    static const Shader & horizon();
    static const Shader & particle();

};

#endif