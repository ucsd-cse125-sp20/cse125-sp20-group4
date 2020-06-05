#include <stdexcept>

#include "EventClasses/SoundEvent.h"
#include "logger.h"
#include "util/Sanitizer.h"

const std::string SoundEvent::TAG = "SoundEvent";

static const auto LOGGER = getLogger( "SoundEvent" );

static constexpr char FIELD_TAG = '|';

/* Static helpers */

static std::vector<Sanitizer::CharMapping> specialCharacters() {

    std::vector<Sanitizer::CharMapping> charMap;

    ADD_CHAR_MAPPING( charMap, FIELD_TAG, "pipe" );

    return charMap;

}

/* Used to sanitize messages */
static const Sanitizer sanitizer( specialCharacters() );

/* Constructors */

SoundEvent::SoundEvent( const std::string & sound, const glm::vec3 & position, const bool isPositional ) :
    Event( "", Event::EventType::Sound ), sound( sound ), position( position ), positional( positional ) {

    if ( sound.empty() ) {
        throw std::invalid_argument( "Sound path cannot be empty." );
    }

}

SoundEvent::SoundEvent( const std::string & sound, const glm::vec3 & position ) : SoundEvent( sound, position, true ) {}

SoundEvent::SoundEvent( const std::string & sound ) : SoundEvent( sound, glm::vec3( 0.0f ), false ) {}

/* Public methods */

const std::string & SoundEvent::getSound() const {

    return sound;

}

bool SoundEvent::isPositional() const {

    return positional;

}

const glm::vec3 & SoundEvent::getPosition() const {

    return position;

}

std::string SoundEvent::serialize() const {

    std::string s = TAG + ":" + sanitizer.sanitize( sound );
    if ( isPositional() ) {
        const glm::vec3 & pos = getPosition();
        s += FIELD_TAG + std::to_string( pos.x ) + ',' + std::to_string( pos.y ) + ',' + std::to_string( pos.z );
    }
    return s;

}

std::shared_ptr<SoundEvent> SoundEvent::deserialize( const std::string & serialized ) {

    size_t tagPos = serialized.find( ':' );
    if ( tagPos == std::string::npos ) {
        LOGGER->error( "Invalid serialized event: '{}'", serialized );
        return nullptr;
    }

    if ( serialized.substr( 0, tagPos ) != TAG ) {
        LOGGER->error( "Not a sound event: '{}'", serialized );
        return nullptr;
    }
    size_t soundPos = tagPos + 1;

    size_t fieldPos = serialized.find( FIELD_TAG, soundPos );
    if ( fieldPos == std::string::npos ) {
        return std::make_shared<SoundEvent>( serialized.substr( soundPos ) );
    } else {
        size_t xPos = fieldPos + 1;
        size_t xEndPos = serialized.find( ',', xPos );
        if ( xEndPos == std::string::npos ) {
            LOGGER->error( "Malformed position: '{}'", serialized.substr( xPos ) );
            return nullptr;
        }
        size_t yPos = xEndPos + 1;

        size_t yEndPos = serialized.find( ',', yPos );
        if ( yEndPos == std::string::npos ) {
            LOGGER->error( "Malformed position: '{}'", serialized.substr( xPos ) );
            return nullptr;
        }
        size_t zPos = yEndPos + 1;

        try {
            float x = std::stof( serialized.substr( xPos, xEndPos - xPos ) );
            float y = std::stof( serialized.substr( yPos, yEndPos - yPos ) );
            float z = std::stof( serialized.substr( zPos ) );

            return std::make_shared<SoundEvent>( serialized.substr( soundPos, fieldPos - soundPos ), glm::vec3( x, y, z ) );
        } catch ( std::invalid_argument & e ) {
            LOGGER->error( "Malformed position: '{}'", serialized.substr( xPos ) );
            return nullptr;
        }
    }

}