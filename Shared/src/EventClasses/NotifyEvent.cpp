#include "EventClasses/Server/NotifyEvent.h"
#include "logger.h"

const std::string NotifyEvent::TAG = "NotifyEvent";
static const std::string HEADER = NotifyEvent::TAG + ":";

static constexpr char END_TAG = '|';
static constexpr char DELIMITER = ';';

NotifyEvent::NotifyEvent(std::string id) : Event(id, Event::EventType::JEvent) {}

std::shared_ptr<NotifyEvent> NotifyEvent::deserialize(const std::string& serialized) {
    if (serialized.substr(0, HEADER.size()) != HEADER) {
        throw std::invalid_argument("Not a valid NotifyEvent serial.");
    }
    size_t end = serialized.find(END_TAG);
    size_t pos = serialized.find(":");
    size_t last = pos + 1;
    std::string id = serialized.substr(last, end - last);
    auto log = getLogger("NotifyEvent");
    log->debug("Just created a Notify event");
    return std::shared_ptr<NotifyEvent>(new NotifyEvent(id));
}

std::string NotifyEvent::serialize() const {
    return HEADER + Event::serialize() + END_TAG;
}

void NotifyEvent::apply(std::shared_ptr<Object> object) const {}