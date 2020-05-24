#pragma once
#include "movingobject.h"
#include <memory>

class Player : public MovingObject {
private:
    //inventory??
    int disregard;
    int health;
    std::shared_ptr<Object> heldItem;
    int money;

public:
    Player(const Player& player);
    Player(std::string id);
    Player(std::string id, float x, float y, float z);
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length);
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ);
    //(de)serialize functions
    virtual std::string serialize() const override;

    int getMoney();
    void setMoney(int newMoney);
    void addMoney(int amount);
    void subtractMoney(int amount);
    
    std::shared_ptr<Object> getHeldItem();
    void setHeldItem(std::shared_ptr<Object> newObject);
    void dropItem();

    void setHealth(int newHealth);
    int getHealth();

    static Player& deserialize(std::string serial);
};