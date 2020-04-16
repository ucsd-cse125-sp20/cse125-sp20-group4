#pragma once
class Object {
private:
    int x, y, z;
    float directionHorizontal, directionVertical;
    int id;
    // mesh
public:
    //constructor
    Object(int id);
    Object(int id, int x, int y, int z);
    Object(int id, int x, int y, int z, float hor, float ver);

    //setters
    void setPositionX(int x);
    void setPositionY(int y);
    void setPositionZ(int z);
    void setPosition(int x, int y, int z);
    void setDirectionHorizontal(float hor);
    void setDirectionVertical(float ver);
    void setDirection(float hor, float ver);

    //getters
    int getId();
    int getPositionX();
    int getPositionY();
    int getPositionZ();
    float getDirectionHorizontal();
    float getDirectionVertical();
};