#ifndef ENTITY_H
#define ENTITY_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "drawing/Model.h"
#include "drawing/model/Axis.h"

class Entity {

    public:
    Entity( const Model * const model, const glm::vec3 position, const glm::vec3 direction, const float scale = 1.0f, const bool axisEnabled = true, const float axisScale = 0.1f );
    virtual ~Entity();

    const Model * const model;

    const glm::vec3 & getPosition() const;
    const glm::vec3 & getDirection() const;
    const float & getScale() const;

    virtual void setPosition( const glm::vec3 & position );
    virtual void setDirection( const glm::vec3 & direction );
    virtual void setScale( const float & scale );

    virtual void draw( const glm::mat4x4 & view ) const;

    protected:
    glm::mat4x4 modelMatrix;

    glm::vec3 position;
    glm::vec3 direction;
    float scale;

    const Axis axis;

    virtual void updateModelMatrix();

};

#endif // !ENTITY_H