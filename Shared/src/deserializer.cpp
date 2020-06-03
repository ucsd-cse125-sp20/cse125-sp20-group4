#include "deserializer.h"
#include "gamestate.h"
#include "logger.h"
#include "ObjectClasses/object.h"
#include "EventClasses/events.h"
#include "ObjectClasses/Factories/factories.h"
#include "EventClasses/Factories/eventmovementfactories.h"
#include "EventClasses/Factories/eventstopfactories.h"
#include "EventClasses/Factories/RotateEventFactory.h"
#include "EventClasses/Factories/gamestateeventfactories.h"
#include "EventClasses/Factories/UpdateEventFactory.h"
#include "EventClasses/Factories/DeleteEventFactory.h"
#include "EventClasses/Factories/NotifyEventFactory.h"

#include "phases/updatephasefactory.h"
#include <cstddef>
#include <stdexcept>


Deserializer::Deserializer() {
    this->gameMapping.insert(std::make_pair("Player", std::make_unique<PlayerFactory>()));
    this->gameMapping.insert(std::make_pair("Shelf", std::make_unique<ShelfFactory>()));
    this->gameMapping.insert(std::make_pair("Barricade", std::make_unique<BarricadeFactory>()));
    this->gameMapping.insert(std::make_pair("Enemy", std::make_unique<EnemyFactory>()));

    this->gameMapping.insert(std::make_pair("RedObject", std::make_unique<RedItemFactory>()));
    this->gameMapping.insert(std::make_pair("GreenObject", std::make_unique<GreenItemFactory>()));
    this->gameMapping.insert(std::make_pair("BlueObject", std::make_unique<BlueItemFactory>()));
    
    this->eventMapping.insert(std::make_pair("MoveLeft", std::make_unique<MoveLeftEventFactory>()));
    this->eventMapping.insert(std::make_pair("MoveRight", std::make_unique<MoveRightEventFactory>()));
    this->eventMapping.insert(std::make_pair("MoveForward", std::make_unique<MoveForwardEventFactory>()));
    this->eventMapping.insert(std::make_pair("MoveBackward", std::make_unique<MoveBackwardEventFactory>()));
    this->eventMapping.insert(std::make_pair("StopLeft", std::make_unique<StopLeftEventFactory>()));
    this->eventMapping.insert(std::make_pair("StopRight", std::make_unique<StopRightEventFactory>()));
    this->eventMapping.insert(std::make_pair("StopForward", std::make_unique<StopForwardEventFactory>()));
    this->eventMapping.insert(std::make_pair("StopBackward", std::make_unique<StopBackwardEventFactory>()));
    this->eventMapping.insert(std::make_pair("RotateEvent", std::make_unique<RotateEventFactory>() ) );
    this->eventMapping.insert(std::make_pair(UpdateEvent::TAG, std::make_unique<UpdateEventFactory>() ) );
    this->eventMapping.insert(std::make_pair(DeleteEvent::TAG, std::make_unique<DeleteEventFactory>()));
    this->eventMapping.insert(std::make_pair("NotifyEvent", std::make_unique<NotifyEventFactory>()));
    this->eventMapping.insert(std::make_pair("PlaceEvent", std::make_unique<PlaceEventFactory>()));
    this->eventMapping.insert(std::make_pair("PickUpEvent", std::make_unique<PickUpEventFactory>()));
    this->eventMapping.insert(std::make_pair(ReadyEvent::TAG, std::make_unique<ReadyEventFactory>()));
    this->eventMapping.insert(std::make_pair(JoinEvent::TAG, std::make_unique<JoinEventFactory>()));
    this->eventMapping.insert(std::make_pair(UseEvent::TAG, std::make_unique<UseEventFactory>()));

    this->eventMapping.insert(std::make_pair("PhaseUpdate", std::make_unique<UpdatePhaseFactory>()));
}

std::string Deserializer::deserializeUpdates(std::string serial, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Object>>> res) {
    // get the tag
    auto log = getLogger("Deserialize");
    size_t pos = serial.find(":");
    size_t end = serial.find("|");
    if ( end == std::string::npos ) {
        return serial; // Not a full message
    }
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    log->trace("Deserializing Updates");
    if (tag.compare("GameState") != 0) {
        //error
        throw "Deserializer: Non gamestate serialization passed in";
    }
    std::string delimiter = ";";
    std::string token;
    // loop over the string while there is still a delimiter
    while ((pos = serial.find(delimiter,last)) != std::string::npos && pos<end) {
        token = serial.substr(last, pos-last);
        last = pos + 1;
        //deserialize the object
        std::shared_ptr<Object> obj = deserializeObject(token);
        std::pair < std::string, std::shared_ptr<Object>> pair(std::string(obj->getId()), obj);
        res->insert(pair);
    }
    return serial.substr( end + 1 ); // Tail end of the message may be part of the next message
}

std::shared_ptr<Event> Deserializer::deserializeEvent( std::string serial ) {
    auto log = getLogger("Deserializer");
    
    size_t tagPos = serial.find( ":" );
    if ( tagPos == std::string::npos ) {
        throw std::invalid_argument( "Not a valid event serial." );
    }
    std::string tag = serial.substr( 0, tagPos );
    std::string input( serial );

    auto factory = eventMapping.find( tag );
    if (factory == eventMapping.end()) {
        throw std::invalid_argument("Unrecognized event type. " + tag);
    }

    log->trace( "Deserializing event {}", tag );
    return factory->second->create( input );
}
std::shared_ptr <Object> Deserializer::deserializeObject( std::string serial ) {

    size_t tagPos = serial.find( ":" );
    if ( tagPos == std::string::npos ) {
        throw std::invalid_argument( "Not a valid object serial." );
    }
    std::string tag = serial.substr( 0, tagPos );
    std::string input( serial );

    auto factory = gameMapping.find( tag );
    if ( factory == gameMapping.end() ) {
        throw std::invalid_argument( "Unrecognized object type." );
    }

    getLogger( "Deserializer" )->trace( "Deserializing object {}", tag );
    return factory->second->create( input );
}