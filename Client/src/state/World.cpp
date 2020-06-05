#pragma warning(disable:4201)

#include <stdexcept>

#include <EventClasses/UpdateEvent.h>
#include <EventClasses/Server/DeleteEvent.h>
#include <logger.h>
#include "ObjectClasses/objects.h"

#include "Window.h"
#include "state/World.h"
#include "Drawing/model/RectangularCuboid.h"
#include "state/CameraEntity.h"
#include "Drawing/model/LoadedModel.h"


static const auto LOGGER = getLogger( "World" );

/* Constructor and destructor */

World::World() : entities() {
    phase.state = READY_STATE;
    phase.count = 0;
}

World::~World() {

    for ( auto it = entities.begin(); it != entities.end(); it++ ) {

        delete( it->second ); // Delete any leftover entities

    }

}

/* Public functions */

void World::draw( const glm::mat4x4 & toView ) const {

    LOGGER->trace( "Starting entity draw." );
    for ( auto it = entities.begin(); it != entities.end(); it++ ) {
        it->second->draw( toView );

    }
    LOGGER->trace( "Entity draw done." );


    LOGGER->trace("Starting particle draw.");
    Window::pmanager->draw( toView, Window::cam->getPos());

    LOGGER->trace("Particle draw done.");
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

    //create particle when the entity is deleted
    Window::pmanager->addExplosion(e);

    return e;

}

void World::createNewEntity(std::shared_ptr<Object> entity,std::string id) {
    if (entity->getId().compare(id)==0) {
        addEntity(new CameraEntity(id, 0.0f, new RectangularCuboid(Window::tmanager->get("wall"), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), entity->getPosition(), entity->getOrientation(), 1.0f, false));
        LOGGER->warn("Created Player camera entity");
    } else {
        addEntity(new Entity(entity->getId(), new RectangularCuboid(Window::tmanager->get("wall"), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), entity->getPosition(), entity->getOrientation()));
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
                if (it->first != id) {
                    entity->setDirection(it->second->getOrientation());
                }
                entity->setPosition( it->second->getPosition(), glm::all( glm::epsilonEqual( entity->getDirection(), it->second->getOrientation(), 0.5f) ) );
            } else {
                LOGGER->debug("Couldn't find entity '{}'.", it->second->getTag());
                if (it->first.compare(id) == 0) {
                    LOGGER->debug("Making a player at pos ({},{},{})", it->second->getPositionX(), it->second->getPositionY(), it->second->getPositionZ());
                    auto model = new LoadedModel("Models/shopper.dae", Window::tmanager->get("shopper"), Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
                    entity = new CameraEntity(it->second->getId(), 1.0f, (model), it->second->getPosition(), it->second->getOrientation(), 0.5f);
                    addEntity(entity);
                    Window::pmanager->addTrail(entity);
                } else if (it->second->getTag().compare("Player") == 0) {
                    LOGGER->debug("Making a Player");
                    auto model = new LoadedModel("Models/shopper.dae", Window::tmanager->get("shopper"), Shaders::phong());
                    size_t found = 0;
                    glm::vec3 colorVec = glm::vec3(1.0f, 1.0f, 1.0f);
                    found = it->second->getId().find("0");
                    if (found != std::string::npos) {
                        colorVec = glm::vec3(1.0f, 0.0f, 0.0f);
                    }
                    found = it->second->getId().find("1");
                    if (found != std::string::npos) {
                        colorVec = glm::vec3(1.0f, 1.0f, 0.0f);

                    }
                    found = it->second->getId().find("2");

                    if (found != std::string::npos) {
                        colorVec = glm::vec3(0.0f, 1.0f, 0.0f);

                    }
                    found = it->second->getId().find("3");
                    if (found != std::string::npos) {
                        colorVec = glm::vec3(0.0f, 1.0f, 1.0f);

                    }
                    found = it->second->getId().find("4");
                    if (found != std::string::npos) {
                        colorVec = glm::vec3(0.0f, 0.0f, 1.0f);

                    }
                    model->setColor(colorVec);
                    entity = new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.5f);
                    addEntity(entity);
                    Window::pmanager->addTrail(entity);
                } else if (it->second->getTag().compare("Enemy") == 0) {
                    LOGGER->debug("Making a Enemy");
                    auto model = new LoadedModel("Models/shopper.dae", Window::tmanager->get("shopper"), Shaders::phong());
                    if (it->first.find("red") != std::string::npos) {
                        model->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
                    }
                    else if (it->first.find("green") != std::string::npos) {
                        model->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
                    }
                    else {
                        model->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
                    }
                    entity = new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.5f);
                    addEntity(entity);
                    //Window::pmanager->addTrail(entity);
                } else if (it->second->getTag().compare("Barricade") == 0) {
                    LOGGER->debug("Making a barricade");
                    auto model = new LoadedModel("Models/barrier.dae", Window::tmanager->get("barrier"), Shaders::phong());
                    //model->setColor(glm::vec3(0.6f, 0.3f, 0.0f));
                    model->setColor(glm::vec3(1.0f));
                    addEntity(new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.5f));
                } else if (it->second->getTag().compare("Shelf") == 0) {

                    auto shelf = std::dynamic_pointer_cast<Shelf>(it->second);
                    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);

                    LoadedModel* model;
                    if (shelf->isCorner == 0) {
                        LOGGER->debug("Making a shelf");
                        // its a shelf
                        if (shelf->getFactory() != nullptr) {
                            if (shelf->getItem()->getTag().compare("Barricade") == 0) {
                                color = glm::vec3(0.6f, 0.3f, 0.0f);
                                model = new LoadedModel("Models/freezer.dae", Window::tmanager->get("default"), Shaders::phong());
                            } else if (shelf->getItem()->getTag().compare("Red") == 0) {
                                color = glm::vec3(1.0f, 0.0f, 0.0f);
                                model = new LoadedModel("Models/freezer.dae", Window::tmanager->get("default"), Shaders::phong());
                            } else if (shelf->getItem()->getTag().compare("Green") == 0) {
                                color = glm::vec3(0.0f, 1.0f, 0.0f);
                                model = new LoadedModel("Models/freezer.dae", Window::tmanager->get("default"), Shaders::phong());
                            } else if (shelf->getItem()->getTag().compare("Blue") == 0) {
                                color = glm::vec3(0.0f, 0.0f, 1.0f);
                                model = new LoadedModel("Models/freezer.dae", Window::tmanager->get("default"), Shaders::phong());
                            } 
                        } else {
                            model = new LoadedModel("Models/edge_shelf.dae", Window::tmanager->get("default"), Shaders::phong());
                        }
                    } else {
                        LOGGER->debug("Making a corner shelf");
                        // its a corner
                        model = new LoadedModel("Models/corner_shelf.dae", Window::tmanager->get("default"), Shaders::phong());
                    }
                    if (model != nullptr) {
                        model->setColor(color);

                        addEntity(new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.5f));
                    }
                } else if (it->second->getTag().compare("ToiletPaper") == 0) {
                    LOGGER->debug("Making a tp");
                    auto model = new LoadedModel("Models/tp.dae", Window::tmanager->get("default"), Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
                    addEntity(new Entity(it->second->getId(), (model), it->second->getPosition(), it->second->getOrientation(), 0.5f));
                } else {
                    LOGGER->debug("Making a spawner");
                    addEntity(new Entity(it->second->getId(), new LoadedModel("Models/spawner.dae", Window::tmanager->get("default"), Shaders::phong()), it->second->getPosition(), it->second->getOrientation(), 0.5f ));
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