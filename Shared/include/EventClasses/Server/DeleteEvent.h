#pragma once
#include "EventClasses/event.h"
#include <string>
#include <vector>
class DeleteEvent : public Event
{
public:
    std::vector<std::string> ids;
    DeleteEvent(std::vector<std::string> id);

    void apply(std::shared_ptr<Object> object) const;
    std::string serialize() const;
    static std::shared_ptr<DeleteEvent> deserialize(const std::string& serialized);
    static const std::string TAG;

};