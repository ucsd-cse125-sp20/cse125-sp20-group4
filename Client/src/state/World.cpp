#pragma warning(disable:4201)

#include <stdexcept>

#include <EventClasses/UpdateEvent.h>
#include <EventClasses/Server/DeleteEvent.h>
#include <logger.h>
#include "ObjectClasses/objects.h"

#include "state/World.h"
#include "Drawing/model/RectangularCuboid.h"
#include "state/CameraEntity.h"
#include "Drawing/model/LoadedModel.h"


static const auto LOGGER = getLogger( "World" );

/* Constructor and destructor */

World::World() : entities(), phase(World::Phase::ROUND) {}

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

    LOGGER->debug("Trying to Remove entity '{}' from world.", name);
    auto found = entities.find( name );
    if ( found == entities.end() ) {
        return nullptr;
    }

    LOGGER->debug( "Removing entity '{}' to world.", name );
    Entity * e = found->second;
    entities.erase( name );
    return e;

}
void World::createNewEntity(std::shared_ptr<Object> entity,std::string id) {
    if (entity->getId().compare(id)==0) {
        addEntity(new CameraEntity(id, 0.0f, new RectangularCuboid(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), entity->getPosition(), entity->getOrientation(), 1.0f, false));
        LOGGER->warn("Created Player camera entity");
    } else {
        addEntity(new Entity(entity->getId(), new RectangularCuboid(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), entity->getPosition(), entity->getOrientation()));
    }

}

void World::handleUpdates( const std::shared_ptr<Event> & e, std::string id ) {

    const std::unordered_map<std::string, std::shared_ptr<Object>>& map = std::dynamic_pointer_cast<UpdateEvent>(e)->updates;
    switch (e->getType())
    {
    case Event::EventType::GEvent:
        LOGGER->debug("Number of updates: {}", map.size());

        for (auto it = map.begin(); it != map.end(); it++) {
            auto entity = this->getEntity(it->first);
            if (entity != nullptr) {
                LOGGER->debug("Updating entity '{}'.", it->second->getId());
                if (it->first.compare(id) != 0) {
                    entity->setDirection(it->second->getOrientation());
                }
                entity->setPosition(it->second->getPosition());
            } else {
                LOGGER->debug("Couldn't find entity '{}'.", it->second->getId());
                LOGGER->debug("Yeeting this {}, {}", map.at(it->second->getId())->serialize(), std::dynamic_pointer_cast<std::shared_ptr<Barricade>>(map.at(it->second->getId())) != nullptr);


                if (it->first.compare(id)==0) {
                    LOGGER->debug("Making a player");
                    auto model = new LoadedModel("Models/shopper.dae", Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 0.0f, 0));
                    addEntity(new CameraEntity(it->second->getId(),0.2f, (model), it->second->getPosition(), it->second->getOrientation(),0.05f));
                } else if (map.at(it->second->getId())->serialize().find("Barricade") != std::string::npos) {
                    LOGGER->debug("Making a barricade");
                    auto model = new LoadedModel("Models/barrier.dae", Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 0.0f, 0));
                    addEntity(new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.4f));
                }
                else if (map.at(it->second->getId())->serialize().find("Shelf") != std::string::npos) {
                    LOGGER->debug("Making a shelf");

                    auto model = new LoadedModel("Models/edge_shelf.dae", Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 1.0f, 0));
                    addEntity(new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.4f));
                } else {
                    LOGGER->debug("Making a cube");

                    addEntity(new Entity(it->second->getId(), new RectangularCuboid(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), it->second->getPosition(), it->second->getOrientation()));
                }
            }
        }
        break;
    case Event::EventType::UEvent:
        std::shared_ptr<DeleteEvent> de = std::dynamic_pointer_cast<DeleteEvent>(e);
        LOGGER->debug("DELETE TIME: {}", de->ids.size());
        for (auto it = de->ids.begin(); it != de->ids.end(); it++) {
            this->removeEntity(*it);
        }
        break;
    }
    
}