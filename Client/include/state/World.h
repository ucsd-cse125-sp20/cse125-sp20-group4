#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>

#include <glm/mat4x4.hpp>

#include "state/Entity.h"

class World {

    public:
    World();
    ~World();

    void draw( const glm::mat4x4 & toView ) const;
    Entity * getEntity( const std::string & key ) const;
    void addEntity( const std::string & key, Entity * const entity );
    Entity * removeEntity( const std::string & key );

    protected:
    std::unordered_map<std::string, Entity *> entities;

};

#endif // !WORLD_H