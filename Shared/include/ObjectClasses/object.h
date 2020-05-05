#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object {
private:
    glm::vec3 position;
    glm::vec3 orientation;
    std::string id;
    // mesh
public:
    //constructor
    Object(std::string id);
    Object(std::string id, float x, float y, float z);
    Object(std::string id, float x, float y, float z, float dirX, float dirY, float dirZ);

    //setters
    void setId(std::string inputId);
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setPosition(float x, float y, float z);
    void setOrientationX(float orientationX);
    void setOrientationY(float orientationY);
    void setOrientationZ(float orientationZ);
    void setOrientation(float orientationX, float orientationY, float orientationZ);

    //getters
    std::string getId() const;
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;
    glm::vec3 getPosition() const;
    float getOrientationX() const;
    float getOrientationY() const;
    float getOrientationZ() const;
    glm::vec3 getOrientation() const;
    virtual float getNextPositionX() const;
    virtual float getNextPositionY() const;
    virtual float getNextPositionZ() const;

    //(de)serialize functions
    virtual std::string serialize();
};