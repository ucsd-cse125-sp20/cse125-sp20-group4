#ifndef MODEL_H
#define MODEL_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Model {

    public:
    virtual ~Model() {};

    virtual void draw( const glm::mat4x4 & toView, const glm::vec3 & direction ) const = 0;
    virtual void draw( const glm::mat4x4 & toView ) const;

};

#endif // !MODEL_H