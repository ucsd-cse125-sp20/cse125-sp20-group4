#pragma once
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <memory>
#include "timer.h"
#include "EventClasses/event.h"
#include "ObjectClasses/objects.h"
#include "MapRep.h"
class GameState
{
private:
    std::map<std::string, std::shared_ptr<Object>> gameObjects;
    std::map<std::string, std::shared_ptr<Timer>> timers;
    std::vector<std::string> deletedIds;
    int nextId;
    bool dirty;

public:
    bool deletes;
    MapRep* map;
    GameState();
    void createObject(std::shared_ptr<Object> obj);
    void createObject(std::shared_ptr<Object> obj, std::string id);
    std::shared_ptr<Object> getObject(std::string id);
    void deleteObject(std::string id);
    void createTimer(std::string id, double duration, std::function<void()>);
    void deleteTimer(std::string id);
    std::map<std::string, std::function<void()>> updateTimers();
    void updateState();
    void applyEvent(std::shared_ptr<Event> event);
    std::string serialize();
    void initialize();
    std::string getUpdates();
    std::vector<std::string> getDeletions();
    void addDeletions(std::string id);
    bool isDirty();
    void setDirty(bool dty);
    void makeDirty();
    void resetDirty();
    void checkCollisions(std::string id, std::shared_ptr<MovingObject> object);
    const std::map<std::string, std::shared_ptr<Object>> & getGameObjects() const;

};