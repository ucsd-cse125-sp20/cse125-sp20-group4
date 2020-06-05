#pragma once
#include <memory>
#include <string>
#include "ObjectClasses/object.h"

class Event {
public:
    enum class EventType { GEvent, OEvent, UEvent, JEvent, PEvent };
    Event( const std::string & objectId, const EventType type );
    const Event::EventType getType() const;
    const std::string & getObjectId() const;
    virtual std::string serialize() const;

private:
    const std::string objectId;
    const EventType type;
};