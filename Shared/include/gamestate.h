#pragma once
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include "timer.h"
#include "ObjectClasses/player.h"
#include "EventClasses/event.h"
#include <memory>
class GameState
{
private:
    std::map<std::string, std::shared_ptr<Object>> gameObjects;
    std::map<std::string, std::shared_ptr<Timer>> timers;
    int nextId;
    bool dirty;

public:
    GameState();
    void createObject(std::shared_ptr<Object> obj);
    void createObject(std::shared_ptr<Object> obj, std::string id);
    void deleteObject(std::string id);
    void createTimer(std::string id, double duration, std::function<void()>);
    void deleteTimer(std::string id);
    std::map<std::string, std::function<void()>> updateTimers();
    void updateState();
    void applyEvent(std::shared_ptr<Event> event);
    std::string serialize();
    void initialize(std::string file = "");
    std::string getUpdates();
    bool isDirty();
    void setDirty(bool dty);
    void checkCollisions(std::string id, std::shared_ptr<MovingObject> object);

};