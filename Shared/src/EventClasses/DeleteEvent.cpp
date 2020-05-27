#include "EventClasses/Server/DeleteEvent.h"
#include "logger.h"

const std::string DeleteEvent::TAG = "DeleteEvent";
static const std::string HEADER = DeleteEvent::TAG + ":";

static constexpr char END_TAG = '|';

DeleteEvent::DeleteEvent(const std::string id) : Event(id, Event::EventType::UEvent) {}

std::shared_ptr<DeleteEvent> DeleteEvent::deserialize(const std::string& serialized) {
    if (serialized.substr(0, HEADER.size()) != HEADER) {
        throw std::invalid_argument("Not a valid DeleteEvent serial.");
    }
    size_t pos = serialized.find(":");
    size_t last = pos + 1;
    // id
    pos = serialized.find(END_TAG, last);
    std::string id = serialized.substr(last, pos - last);
    
    return std::shared_ptr<DeleteEvent>(new DeleteEvent(id));
}

std::string DeleteEvent::serialize() const {
    return HEADER + this->getObjectId() + END_TAG;
}

void DeleteEvent::apply(std::shared_ptr<Object> object) const {}