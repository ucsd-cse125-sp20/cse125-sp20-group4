#include "glm/gtc/matrix_transform.hpp"

#include "logger.h"
#include "state/Entity.h"

static const auto LOGGER = getLogger( "Entity" );

/* Constructor */

Entity::Entity( const std::string & name, const Model * const model, const glm::vec3 position, const glm::vec3 direction, const float scale, const bool axisEnabled, const float axisScale ) :
        name( name ), model( model ), position( position ), direction( glm::normalize( direction ) ), scale( scale ), axis( axisEnabled, axisScale ) {

    updateModelMatrix();

}

Entity::~Entity() {

    delete( model ); // For now, also delete model

}

/* Getters */

const glm::vec3 & Entity::getPosition() const {

    return position;

}

const glm::vec3 & Entity::getDirection() const {

    return direction;

}

const float & Entity::getScale() const {

    return scale;

}

/* Setters */

void Entity::setPosition( const glm::vec3 & pos ) {

    auto log = getLogger("Entity");
    log->info("Setting position from ({}, {}, {}) to ({}, {}, {})", position.x, position.y, position.z, pos.x, pos.y, pos.z);
    position = pos;
    updateModelMatrix();

}

void Entity::setDirection( const glm::vec3 & dir ) {

    direction = glm::normalize( dir );
    // Direction rotation handled by Model class so no need to update matrix

}

void Entity::setScale( const float & s ) {

    scale = s;
    updateModelMatrix();

}

/* Public functions */

void Entity::draw( const glm::mat4x4 & view ) const {

    // Delegate to model
    model->draw( modelMatrix, view, direction );

    // Draw debugging axis
    axis.draw( modelMatrix, view, direction );

}

/* Protected functions */

void Entity::updateModelMatrix() {

    static const glm::mat4x4 I( 1.0f );
    glm::mat4x4 scaleMatrix = glm::scale( I, glm::vec3( scale ) );
    glm::mat4x4 translateMatrix = glm::translate( I, position );

    modelMatrix = translateMatrix * scaleMatrix;

}