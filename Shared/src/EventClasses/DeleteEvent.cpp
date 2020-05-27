#include "EventClasses/Server/DeleteEvent.h"
#include "logger.h"

const std::string DeleteEvent::TAG = "DeleteEvent";
static const std::string HEADER = DeleteEvent::TAG + ":";

static constexpr char END_TAG = '|';
static constexpr char DELIMITER = ';';

DeleteEvent::DeleteEvent(std::vector<std::string> id) : Event("ignore", Event::EventType::UEvent){
    ids = id;
}

std::shared_ptr<DeleteEvent> DeleteEvent::deserialize(const std::string& serialized) {
    if (serialized.substr(0, HEADER.size()) != HEADER) {
        throw std::invalid_argument("Not a valid DeleteEvent serial.");
    }
    size_t end = serialized.find(END_TAG);
    size_t pos = serialized.find(":");
    size_t last = pos + 1;
    std::vector<std::string> res;
    std::string token;
    // loop over the string while there is still a delimiter
    while ((pos = serialized.find(DELIMITER, last)) != std::string::npos && pos < end) {
        token = serialized.substr(last, pos - last);
        last = pos + 1;
        res.push_back(token);
    }
    
    return std::shared_ptr<DeleteEvent>(new DeleteEvent(res));
}

std::string DeleteEvent::serialize() const {
    std::string res = HEADER;
    for (auto it = ids.begin(); it != ids.end(); it++) {
        if (res.compare(HEADER) == 0) {
            res += *it;
        } else {
            res += DELIMITER + *it;
        }
    }
    res += END_TAG;
    
    return res;
}

void DeleteEvent::apply(std::shared_ptr<Object> object) const {}