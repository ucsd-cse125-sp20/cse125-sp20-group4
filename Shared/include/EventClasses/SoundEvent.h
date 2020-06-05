#ifndef SOUND_EVENT_H
#define SOUND_EVENT_H

#include <glm/vec3.hpp>

#include "EventClasses/event.h"

class SoundEvent : public Event {

    public:
    SoundEvent( const std::string & sound );
    SoundEvent( const std::string & sound, const glm::vec3 & position );

    const std::string & getSound() const;
    bool isPositional() const;
    const glm::vec3 & getPosition() const;

    std::string serialize() const;
    static std::shared_ptr<SoundEvent> deserialize( const std::string & serialized );

    static const std::string TAG;

    private:
    SoundEvent( const std::string & sound, const glm::vec3 & position, const bool positional );

    const std::string sound;
    const bool positional;
    const glm::vec3 position;

};

#endif