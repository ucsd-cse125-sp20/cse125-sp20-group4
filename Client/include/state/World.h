#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <memory>
#include <glm/mat4x4.hpp>

#include <EventClasses/UpdateEvent.h>
#include <ObjectClasses/object.h>

#include "state/Entity.h"

class World {

    public:
    World();
    ~World();
    enum class Phase {READY, END, ROUND};
    Phase phase;
    void draw( const glm::mat4x4 & toView ) const;
    Entity * getEntity( const std::string & name ) const;
    void addEntity( Entity * const entity );
    Entity * removeEntity( const std::string & name );
    void handleUpdates( const std::shared_ptr<Event> & e , std::string id);
    void createNewEntity(std::shared_ptr<Object> entity, std::string id);

    protected:
    std::unordered_map<std::string, Entity *> entities;

};

#endif // !WORLD_H