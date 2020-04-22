#include "glm/gtc/matrix_transform.hpp"

#include "logger.h"
#include "state/Entity.h"

static const auto LOGGER = getLogger( "Entity" );

/* Constructor */

Entity::Entity( const Model * const model, const glm::vec3 position, const glm::vec3 direction, const float scale ) : 
        model( model ), position( position ), direction( direction ), scale( scale ) {

    updateModelMatrix();

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

    position = pos;
    updateModelMatrix();

}

void Entity::setDirection( const glm::vec3 & dir ) {

    direction = dir;
    // Direction rotation handled by Model class so no need to update matrix

}

void Entity::setScale( const float & s ) {

    scale = s;
    updateModelMatrix();

}

/* Public functions */

void Entity::draw( const glm::mat4x4 & toView ) const {

    // Delegate to model
    model->draw( toView * modelMatrix, direction );

}

/* Protected functions */

void Entity::updateModelMatrix() {

    modelMatrix = glm::translate( glm::scale( glm::mat4x4( 1.0f ), glm::vec3( scale ) ), position );

}