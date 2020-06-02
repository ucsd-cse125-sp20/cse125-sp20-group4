#ifndef MODEL_H
#define MODEL_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Model {

    public:
    virtual ~Model() {};

    virtual void draw( const glm::mat4x4 & model, const glm::mat4x4 & view, const glm::vec3 & direction ) const = 0;
    virtual void draw( const glm::mat4x4 & model, const glm::mat4x4 & view ) const;

};

#endif // !MODEL_H