#include <stdexcept>

#include "logger.h"
#include "state/CameraEntity.h"

const auto LOGGER = getLogger( "CameraEntity" );

/* Constructor and destructor */

CameraEntity::CameraEntity( const std::string & cameraName, float camY, const Model * const model, const glm::vec3 position, 
                            const glm::vec3 direction, const float scale, const bool axisEnabled, const float axisScale ) :
        Camera( position + glm::vec3( 0.0f, camY, 0.0f ), direction ), Entity( model, position, direction, scale, axisEnabled, axisScale ), 
        camOffset( 0.0f, camY, 0.0f ), cameraName( cameraName ) {

    LOGGER->info( "Creating new camera entity '{}'.", cameraName );

    if ( cameras.count( cameraName ) > 0 ) {
        throw std::invalid_argument( "A camera with that name already exists." );
    }
    cameras[cameraName] = std::make_pair( this, true );

}

CameraEntity::~CameraEntity() {

    LOGGER->info( "Deleting camera entity '{}'.", cameraName );

    if ( cameras.erase( cameraName ) == 0 ) {
        LOGGER->error( "Entity-attached camera '{}' was unexpectedly deleted." );
    }

}

/* Public methods */

void CameraEntity::update( const glm::vec3 & newPos, const glm::vec3 & newDir ) {

    Camera::update( newPos, newDir );
    Entity::setPosition( newPos - camOffset );
    Entity::setDirection( newDir );

}

void CameraEntity::setPosition( const glm::vec3 & newPos ) {

    Entity::setPosition( newPos );
    Camera::update( newPos + camOffset, dir );

}

void CameraEntity::setDirection( const glm::vec3 & newDir ) {

    Entity::setDirection( newDir );
    Camera::update( pos, newDir );

}