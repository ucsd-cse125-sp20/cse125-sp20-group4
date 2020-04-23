#ifndef CAMERA_H
#define CAMERA_H

#include <unordered_map>

#include <glm/mat4x4.hpp>

class Camera {

    public:
    const glm::vec3 & getPos() const;
    const glm::vec3 & getDir() const;
    const glm::mat4x4 & getV() const;
    const glm::mat4x4 & getToView() const;

    virtual void update( const glm::vec3 & pos, const glm::vec3 & dir );
    void move( const glm::vec3 & pos );
    void rotate( const glm::vec3 & dir );

    static void setZ( float nearZ, float farZ );
    static void setFOV( float fov );
    static void setAspect( float aspect );

    static Camera * getCamera( const std::string & name );
    static Camera * addCamera( const std::string & name, const glm::vec3 & pos, const glm::vec3 & dir );
    static void removeCamera( const std::string & name );

    protected:
    Camera( const glm::vec3 & pos, const glm::vec3 & dir );
    virtual ~Camera() {};

    glm::vec3 pos, dir;

    glm::mat4x4 V;
    glm::mat4x4 toView;

    void update();
    void updateV();

    static float nearZ, farZ, fov, aspect;
    static glm::mat4x4 P;
    // If the boolean is true, camera is managed by something and should not be manually deleted
    static std::unordered_map<std::string, std::pair<Camera *, bool>> cameras;

    static void updateAll();
    static void updateP();


};

#endif !CAMERA_H