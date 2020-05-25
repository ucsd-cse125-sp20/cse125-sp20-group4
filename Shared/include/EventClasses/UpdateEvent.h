#ifndef UPDATE_EVENT_H
#define UPDATE_EVENT_H

#include <unordered_map>

#include "EventClasses/event.h"

class UpdateEvent : public Event {

    public:
    UpdateEvent( const std::unordered_map<std::string, std::shared_ptr<Object>> & updates );
    const std::unordered_map<std::string, std::shared_ptr<Object>> updates;

    void apply( std::shared_ptr<Object> object ) const;
    std::string serialize() const;
    static std::shared_ptr<UpdateEvent> deserialize( const std::string & serialized );

    static const std::string TAG;

};

#endif