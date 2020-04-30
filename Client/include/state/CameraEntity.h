#ifndef CAMERA_ENTITY_H
#define CAMERA_ENTITY_H

#include "state/Camera.h"
#include "state/Entity.h"

class CameraEntity : public Camera, public Entity {

    public:
    CameraEntity( const std::string & cameraName,float camY, const Model * const model, const glm::vec3 position, const glm::vec3 direction, const float scale = 1.0f, const bool axisEnabled = true, const float axisScale = 0.1f );
    ~CameraEntity();

    void update( const glm::vec3 & pos, const glm::vec3 & dir );
    void setPosition( const glm::vec3 & position );
    void setDirection( const glm::vec3 & direction );

    protected:
    const glm::vec3 camOffset;
    const std::string cameraName;

};

#endif