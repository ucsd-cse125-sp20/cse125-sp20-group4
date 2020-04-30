#include <stdexcept>

#include "logger.h"
#include "state/World.h"

static const auto LOGGER = getLogger( "World" );

/* Constructor and destructor */

World::World() : entities() {}

World::~World() {

    for ( auto it = entities.begin(); it != entities.end(); it++ ) {

        delete( it->second ); // Delete any leftover entities

    }

}

/* Public functions */

void World::draw( const glm::mat4x4 & toView ) const {

    LOGGER->trace( "Starting world draw." );
    for ( auto it = entities.begin(); it != entities.end(); it++ ) {

        it->second->draw( toView );

    }
    LOGGER->trace( "World draw done." );

}

Entity * World::getEntity( const std::string & key ) const {

    auto found = entities.find( key );
    return found == entities.end() ? nullptr : found->second;

}

void World::addEntity( const std::string & key, Entity * const entity ) {

    if ( entity == nullptr ) {
        throw std::invalid_argument( "Entity cannot be a null pointer." );
    }
    if ( entities.count( key ) > 0 ) {
        throw std::invalid_argument( "An entitiy with the given key already exists." );
    }

    LOGGER->debug( "Adding entity '{}' to world.", key );
    entities[key] = entity;

}

Entity * World::removeEntity( const std::string & key ) {

    auto found = entities.find( key );
    if ( found == entities.end() ) {
        return nullptr;
    }

    LOGGER->debug( "Removing entity '{}' to world.", key );
    Entity * e = found->second;
    entities.erase( key );
    return e;

}