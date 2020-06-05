#ifndef ENTITY_H
#define ENTITY_H

#include <fmod_studio.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "drawing/Model.h"
#include "drawing/model/Axis.h"

class Entity {

    public:
    Entity( const std::string & name, Model * const model, const glm::vec3 position, const glm::vec3 direction, const float scale = 1.0f, const bool axisEnabled = true, const float axisScale = 0.1f );
    virtual ~Entity();

    const Model * model;
    const std::string name;

    const glm::vec3 & getPosition() const;
    const glm::vec3 & getDirection() const;
    const glm::vec3 & getVelocity() const;
    const float & getScale() const;

    virtual void setPosition( const glm::vec3 & position, bool directionChanged = false );
    virtual void setDirection( const glm::vec3 & direction );
    virtual void setScale( const float & scale );

    virtual void draw( const glm::mat4x4 & view ) const;

    protected:
    glm::mat4x4 modelMatrix;

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 velocity = glm::vec3(0.0f);
    float scale;

    const Axis axis;

    virtual void updateModelMatrix();

    private:
    FMOD::Studio::EventDescription * movingSound;
    FMOD::Studio::EventInstance * movingSoundEvent;

};

#endif // !ENTITY_H