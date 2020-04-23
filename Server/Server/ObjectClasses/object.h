#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object {
private:
    glm::vec3 location;
    glm::vec3 orientation;
    int id;
    // mesh
public:
    //constructor
    Object(int id);
    Object(int id, float x, float y, float z);
    Object(int id, float x, float y, float z, float dirX, float dirY, float dirZ);

    //setters
    void setId(int id);
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setPosition(float x, float y, float z);
    void setOrientationX(float orientationX);
    void setOrientationY(float orientationY);
    void setOrientationZ(float orientationZ);


    void setOrientation(float orientationX, float orientationY, float orientationZ);

    //getters
    int getId();
    float getPositionX();
    float getPositionY();
    float getPositionZ();
    float getOrientationX();
    float getOrientationY();
    float getOrientationZ();
    glm::vec3 getOrientation();

    //(de)serialize functions
    virtual std::string serialize();
};