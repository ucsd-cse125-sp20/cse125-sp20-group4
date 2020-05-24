#pragma once
#include "EventClasses/event.h"

class CreateEvent : public Event
{
    public:
        CreateEvent( const std::string & objectId);
        virtual std::string serialize() const;
        virtual void apply(std::shared_ptr<Object> object) const = 0;
    private:
        // id, pos, direction, type
        const glm::vec3 pos, direction;
        const char type;
};