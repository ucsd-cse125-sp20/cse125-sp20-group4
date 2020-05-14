#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <memory>
#include <glm/mat4x4.hpp>

#include "state/Entity.h"
#include "ObjectClasses/object.h"

class World {

    public:
    World();
    ~World();

    void draw( const glm::mat4x4 & toView ) const;
    Entity * getEntity( const std::string & key ) const;
    void addEntity( const std::string & key, Entity * const entity );
    Entity * removeEntity( const std::string & key );
    void handleUpdates(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Object>>> map);

    protected:
    std::unordered_map<std::string, Entity *> entities;

};

#endif // !WORLD_H