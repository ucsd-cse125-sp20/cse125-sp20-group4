#include <stdexcept>

#include "logger.h"
#include "state/CameraEntity.h"

const auto LOGGER = getLogger( "CameraEntity" );

/* Constructor and destructor */

CameraEntity::CameraEntity( const std::string & name, float camY, Model * const model, const glm::vec3 position, 
                            const glm::vec3 direction, const float scale, const bool axisEnabled, const float axisScale ) :
        Camera( name, position + glm::vec3( 0.0f, camY, 0.0f ), direction ),
        Entity( name, model, position, direction, scale, axisEnabled, axisScale ), 
        camOffset( 0.0f, camY, 0.0f ) {

    LOGGER->info( "Creating new camera entity '{}'.", name );

    if ( cameras.count( name ) > 0 ) {
        throw std::invalid_argument( "A camera with that name already exists." );
    }
    cameras[name] = this;

}

CameraEntity::~CameraEntity() {

    LOGGER->info( "Deleting camera entity '{}'.", name );

    if ( cameras.erase( name ) == 0 ) {
        LOGGER->error( "Entity-attached camera '{}' was unexpectedly deleted." );
    }

}

/* Meta methods */

bool CameraEntity::isFreeCamera() const {

    return false;

}

/* Public methods */

void CameraEntity::update( const glm::vec3 & newPos, const glm::vec3 & newDir ) {

    Camera::update( newPos, newDir );
    Entity::setPosition( newPos - camOffset );
    Entity::setDirection( newDir );

}

void CameraEntity::setPosition( const glm::vec3 & newPos, bool directionChanged ) {

    Entity::setPosition( newPos, directionChanged );
    Camera::update( newPos + camOffset, dir );

}

void CameraEntity::setDirection( const glm::vec3 & newDir ) {

    Entity::setDirection( newDir );
    Camera::update( pos, newDir );

}