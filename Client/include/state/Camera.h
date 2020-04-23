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

    void update( const glm::vec3 & pos, const glm::vec3 & dir );
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

    glm::vec3 pos, dir;

    glm::mat4x4 V;
    glm::mat4x4 toView;

    void update();
    void updateV();

    static float nearZ, farZ, fov, aspect;
    static glm::mat4x4 P;
    static std::unordered_map<std::string, Camera *> cameras;

    static void updateAll();
    static void updateP();


};

#endif !CAMERA_H