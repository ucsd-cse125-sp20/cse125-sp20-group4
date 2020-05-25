#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "logger.h"
#include "state/Camera.h"

#define RADIANS( W ) ( W ) * ( glm::pi<float>() / 180.0f )

const auto LOGGER = getLogger( "Camera" );

/* Static variables */

float Camera::nearZ = 0.1f;
float Camera::farZ = 300.0f;
float Camera::fov = RADIANS( 45.0f );
float Camera::aspect = 0.0f;

glm::mat4x4 Camera::P( 0.0f );
std::unordered_map<std::string, Camera *> Camera::cameras;

/* Constructor */

Camera::Camera( const std::string & name, const glm::vec3 & pos, const glm::vec3 & dir ) : name( name ), pos( pos ), dir( dir ), V( 0.0f ), toView( 0.0f ) {

    updateV();

}

/* Getters */

bool Camera::isFreeCamera() const {

    return true;

}

const glm::vec3 & Camera::getPos() const {

    return pos;

}

const glm::vec3 & Camera::getDir() const {

    return dir;

}

const glm::mat4x4 & Camera::getV() const {

    return V;

}

const glm::mat4x4 & Camera::getToView() const {

    return toView;

}

/* Public instance methods */

void Camera::update( const glm::vec3 & newPos, const glm::vec3 & newDir ) {

    pos = newPos;
    dir = glm::normalize( newDir );
    
    updateV();

}

void Camera::move( const glm::vec3 & newPos ) {

    update( newPos, dir );

}

void Camera::rotate( const glm::vec3 & newDir ) {

    update( pos, newDir );

}

/* Public static methods */

void Camera::setZ( float newNearZ, float newFarZ ) {

    nearZ = newNearZ;
    farZ = newFarZ;
    updateP();

}

void Camera::setFOV( float newFov ) {

    fov = newFov;
    updateP();

}

void Camera::setAspect( float newAspect ) {

    aspect = newAspect;
    updateP();

}

Camera * Camera::getCamera( const std::string & name ) {

    auto it = cameras.find( name );
    if ( it == cameras.end() ) {
        throw std::invalid_argument( "Camera not found." );
    }
    return it->second;

}

Camera * Camera::addCamera( const std::string & name, const glm::vec3 & pos, const glm::vec3 & dir ) {

    LOGGER->info( "Adding new camera '{}'.", name );

    if ( cameras.count( name ) > 0 ) {
        throw std::invalid_argument( "A camera with that name already exists." );
    }

    Camera * cam = new Camera( name, pos, dir );
    cameras[name] = cam;
    return cam;

}

void Camera::removeCamera( const std::string & name ) {

    LOGGER->info( "Removing camera '{}'.", name );

    auto it = cameras.find( name );
    if ( it == cameras.end() ) {
        throw std::invalid_argument( "Camera not found." );
    }

    Camera * cam = it->second;
    if ( !cam->isFreeCamera() ) {
        throw std::invalid_argument( "Cannot manually delete a managed camera." );
    }

    cameras.erase( name );
    delete( cam );

}

/* Protected instance methods */

void Camera::update() {

    toView = P * V;

}

void Camera::updateV() {

    static const glm::vec3 UP( 0.0f, 1.0f, 0.0f );

    glm::vec3 look_at = pos + dir;
    glm::vec3 up = glm::normalize( glm::cross( glm::cross( dir, UP ), dir ) );
    V = glm::lookAt( pos, look_at, up );
    update();

}

/* Protected static methods */

void Camera::updateAll() {

    for ( auto it = cameras.begin(); it != cameras.end(); it++ ) {

        it->second->update();

    }

}

void Camera::updateP() {

    P = glm::perspective( fov, aspect, nearZ, farZ );
    updateAll();

}