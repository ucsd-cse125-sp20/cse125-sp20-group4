#pragma once
#include <string>

class Object {
private:
    int x, y, z;
    float orientationY, orientationX;
    int id;
    // mesh
public:
    //constructor
    Object(int id);
    Object(int id, int x, int y, int z);
    Object(int id, int x, int y, int z, float orientationX, float orientationY);

    //setters
    void setId(int id);
    void setPositionX(int x);
    void setPositionY(int y);
    void setPositionZ(int z);
    void setPosition(int x, int y, int z);
    void setOrientationX(float orientationX); //TODO change orientation to a vector, adding Z
    void setOrientationY(float orientationY);
    void setOrientation(float orientationX, float orientationY);

    //getters
    int getId();
    int getPositionX();
    int getPositionY();
    int getPositionZ();
    float getOrientationY();
    float getOrientationX();
    virtual int getNextPositionX();
    virtual int getNextPositionY();
    virtual int getNextPositionZ();

    //(de)serialize functions
    virtual std::string serialize();
};