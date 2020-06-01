#pragma once
#include "EventClasses/event.h"
#include <string>
class NotifyEvent : public Event
{
public:
    NotifyEvent(std::string id);

    void apply(std::shared_ptr<Object> object) const;
    std::string serialize() const;
    static std::shared_ptr<NotifyEvent> deserialize(const std::string& serialized);
    static const std::string TAG;

};