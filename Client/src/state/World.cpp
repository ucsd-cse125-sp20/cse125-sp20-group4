#include <stdexcept>

#include <EventClasses/UpdateEvent.h>
#include <logger.h>

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

Entity * World::getEntity( const std::string & name ) const {

    auto found = entities.find( name );
    return found == entities.end() ? nullptr : found->second;

}

void World::addEntity( Entity * const entity ) {

    if ( entity == nullptr ) {
        throw std::invalid_argument( "Entity cannot be a null pointer." );
    }
    if ( entities.count( entity->name ) > 0 ) {
        throw std::invalid_argument( "An entitiy with that name already exists." );
    }

    LOGGER->debug( "Adding entity '{}' to world.", entity->name );
    entities[entity->name] = entity;

}

Entity * World::removeEntity( const std::string & name ) {

    auto found = entities.find( name );
    if ( found == entities.end() ) {
        return nullptr;
    }

    LOGGER->debug( "Removing entity '{}' to world.", name );
    Entity * e = found->second;
    entities.erase( name );
    return e;

}

void World::handleUpdates( const std::shared_ptr<UpdateEvent> & e ) {

    const std::unordered_map<std::string, std::shared_ptr<Object>> & map = e->updates;
    LOGGER->debug( "Number of updates: {}", map.size() );

    for ( auto it = map.begin(); it != map.end(); it++ ) {
        //auto entity = this->getEntity(it->second->getId());
        auto entity = this->getEntity( it->first );

        if ( entity != nullptr ) {
            LOGGER->debug( "Updating entity '{}'.", it->second->getId() );
            entity->setDirection( it->second->getOrientation() );
            entity->setPosition( it->second->getPosition() );
        } else {
            LOGGER->debug( "Couldn't find entity '{}'.", it->second->getId() );
        }
    }
}