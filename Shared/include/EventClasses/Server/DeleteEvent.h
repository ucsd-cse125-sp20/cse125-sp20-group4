#pragma once
#include "EventClasses/event.h"

class DeleteEvent : public Event
{
public:
    DeleteEvent(const std::string id);

    void apply(std::shared_ptr<Object> object) const;
    std::string serialize() const;
    static std::shared_ptr<DeleteEvent> deserialize(const std::string& serialized);

    static const std::string TAG;

};