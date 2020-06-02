#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "drawing/Model.h"
#include "drawing/Shaders.h"

class Geometry : public Model {

    public:
    Geometry( const Shader & shaderProgram, GLenum drawMode = GL_TRIANGLES );
    Geometry( const Shader & shaderProgram, const std::vector<glm::vec3> & vertices, const std::vector<glm::vec3> & colors, const std::vector<glm::vec3> & normals, const std::vector<unsigned int> & indices, GLenum drawMode = GL_TRIANGLES );
    ~Geometry();

    virtual void draw( const glm::mat4x4 & model, const glm::mat4x4 & view, const glm::vec3 & direction ) const;

    protected:
    const Shader shaderProgram;
    const GLenum drawMode;
    std::vector<float> data;
    std::vector<unsigned int> indices;
    GLuint VAO, EBO, VBO;
    bool initialized;

    void initialize( const std::vector<glm::vec3> & vertices, const std::vector<glm::vec3> & colors, const std::vector<glm::vec3> & normals, const std::vector<unsigned int> & indices );
    virtual void setup() const {};
    virtual void restore() const {};

};

#endif // !GEOMETRY_H