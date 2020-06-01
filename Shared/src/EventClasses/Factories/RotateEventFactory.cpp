#include "EventClasses/Factories/RotateEventFactory.h"
#include "EventClasses/Object/RotateEvent.h"

std::shared_ptr<Event> RotateEventFactory::create( std::string serial ) {
    
    size_t pos = serial.find( ":" );
    size_t last = pos + 1;
    std::string tag = serial.substr( 0, pos );

    // id
    pos = serial.find( ",", last );
    std::string id = serial.substr( last, pos - last );

    // x
    last = pos + 1;
    pos = serial.find( ",", last );
    float axisX = std::stof( serial.substr( last, pos - last ) );
    // y
    last = pos + 1;
    pos = serial.find( ",", last );
    float axisY = std::stof( serial.substr( last, pos - last ) );
    // z
    last = pos + 1;
    pos = serial.find( ",", last );
    float axisZ = std::stof( serial.substr( last, pos - last ) );

    std::shared_ptr<Event> event = std::make_shared<RotateEvent>( id, glm::vec3( axisX, axisY, axisZ ) );
    return event;

}
