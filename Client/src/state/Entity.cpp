#pragma warning(disable:4201)

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <logger.h>

#include "state/Entity.h"
#include "Window.h"

#define EPSILON 0.0005f
#define SOUND_COOLDOWN 30

static const auto LOGGER = getLogger( "Entity" );

/* Constructor */

Entity::Entity( const std::string & name, const Model * const model, const glm::vec3 position, const glm::vec3 direction, const float scale, const bool axisEnabled, const float axisScale ) :
        name( name ), model( model ), position( position ), direction( glm::normalize( direction ) ), scale( scale ), axis( axisEnabled, axisScale ) {

    updateModelMatrix();

    Window::audioSystem->getEvent( "event:/player_walk", &movingSound );
    movingSoundEvent = nullptr;

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

const glm::vec3& Entity::getVelocity() const {

    return velocity;

}

const float & Entity::getScale() const {

    return scale;

}

/* Setters */

void Entity::setPosition( const glm::vec3 & pos, bool directionChanged ) {

    auto log = getLogger( "Entity" );
    log->trace( "Setting position from ({}, {}, {}) to ({}, {}, {})", position.x, position.y, position.z, pos.x, pos.y, pos.z );

    if ( movingSoundEvent != nullptr ) {
        FMOD_STUDIO_PLAYBACK_STATE state;
        movingSoundEvent->getPlaybackState( &state );
        if ( state == FMOD_STUDIO_PLAYBACK_STOPPED ) {
            LOGGER->trace( "Movement sound ended." );
            movingSoundEvent->release();
            movingSoundEvent = nullptr;
        }
    }

    if ( glm::all( glm::epsilonEqual( pos, position, 0.0001f ) ) ) {
        if ( movingSoundEvent != nullptr && directionChanged ) {
            LOGGER->trace( "Stopping movement sound." );
            movingSoundEvent->stop( FMOD_STUDIO_STOP_IMMEDIATE );
            movingSoundEvent->release();
            movingSoundEvent = nullptr;
        }
    } else {
        if ( movingSoundEvent == nullptr ) {
            LOGGER->trace( "Starting movement sound." );
            movingSound->createInstance( &movingSoundEvent );
            movingSoundEvent->start();
        }
        FMOD_3D_ATTRIBUTES attributes;
        Window::set3DParams( attributes, getPosition(), glm::vec3( 0.0f ), getDirection() );
        FMOD_RESULT res = movingSoundEvent->set3DAttributes( &attributes );
        if ( res != FMOD_OK ) {
            LOGGER->warn( "Error while updating sound event position ({}).", res );
        }
    }

    velocity = pos - position;

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