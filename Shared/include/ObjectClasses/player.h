#pragma once
#include "movingobject.h"
#include <memory>

class Player : public MovingObject {
private:
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
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ, int startingMoney, int hp, std::shared_ptr<Object> held);
    
    const std::string TAG = "Player";
    const std::string& getTag();

    //(de)serialize functions
    virtual std::string serialize() const override;

    int getMoney() const;
    void setMoney(int newMoney);
    void addMoney(int amount);
    void subtractMoney(int amount);
    
    std::shared_ptr<Object> getHeldItem() const;
    void setHeldItem(std::shared_ptr<Object> newObject);
    void dropItem();

    void setHealth(int newHealth);
    int getHealth() const;
    virtual std::shared_ptr<Object> clone() const override;
    static Player& deserialize(std::string serial);
};