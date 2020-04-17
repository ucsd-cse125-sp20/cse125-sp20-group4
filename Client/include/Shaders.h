#ifndef SHADERS_H
#define SHADERS_H

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <string>

typedef GLuint Shader;

class Shaders {

    private:
    /* Modifiable variables */
    static Shader skyboxShader_;
    static Shader normalColoringShader_;
    static Shader curveShader_;
    static Shader flatShader_;
    static Shader phongShader_;
    static Shader horizonShader_;

    static Shader loadShaders( std::string vertex_file, std::string fragment_file );
    static Shader loadShaders( std::string shaderName );

    public:
    static void initializeShaders();
    static void deleteShaders();

    /* Constant getters */
    static const Shader & skyboxShader();
    static const Shader & normalColoringShader();
    static const Shader & curveShader();
    static const Shader & flatShader();
    static const Shader & phongShader();
    static const Shader & horizonShader();

};

#endif