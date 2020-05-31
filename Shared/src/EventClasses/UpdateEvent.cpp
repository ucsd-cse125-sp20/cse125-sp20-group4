#include <stdexcept>

#include "deserializer.h"
#include "EventClasses/UpdateEvent.h"
#include "logger.h"
#include "util/Sanitizer.h"

const std::string UpdateEvent::TAG = "UpdateEvent";
static const std::string HEADER = UpdateEvent::TAG + ":";

static constexpr char DATA_TAG = '#';
static constexpr char END_TAG = '|';

static const auto LOGGER = getLogger( "UpdateEvent" );

/* Static helpers */

static std::vector<Sanitizer::CharMapping> specialCharacters() {

    std::vector<Sanitizer::CharMapping> charMap;

    ADD_CHAR_MAPPING( charMap, DATA_TAG, "hash" );
    ADD_CHAR_MAPPING( charMap, END_TAG, "pipe" );

    return charMap;

}

/* Used to sanitize messages */
static const Sanitizer sanitizer( specialCharacters() );

UpdateEvent::UpdateEvent( const std::unordered_map<std::string, std::shared_ptr<Object>> & updates ) : Event( "ignore", Event::EventType::GEvent){
    //std::unordered_map<std::string, std::shared_ptr<Object>> newMap;
    Deserializer deserializer;
    auto iter = updates.begin();
    while (iter != updates.end()) {
        auto pair = std::pair<std::string, std::shared_ptr<Object>>(iter->first, iter->second->clone()); //needs to make underlying object
        //LOGGER->trace("{}", pair.second->serialize());
        this->updates.insert(pair);
        iter++;
    }
    //this->updates = newMap;
}

std::string UpdateEvent::serialize() const {

    std::string serial = HEADER;
    for ( auto it = updates.cbegin(); it != updates.cend(); it++ ) {
        if (it->second->dirty) {
            serial += sanitizer.sanitize(it->first) + DATA_TAG + sanitizer.sanitize(it->second->serialize()) + END_TAG;
        }

    }
    return serial;

}

static std::shared_ptr<Object> readEntry( const std::string & str, const size_t start, const size_t end, std::string & id ) {

    static Deserializer decoder;

    size_t data = str.find( DATA_TAG, start );
    if ( data == std::string::npos || data > end ) {
        LOGGER->error( "Dropping entry with no data: {}", str.substr( start, end ) );
        return nullptr;
    }
    id = sanitizer.restore( str.substr( start, data - start ) );
    data += 1;
    std::string dat = sanitizer.restore( str.substr( data, end - data ) );

    try {
        return decoder.deserializeObject( dat );
    } catch ( std::invalid_argument & e ) {
        LOGGER->error( "Dropping entry with invalid data ({}): {} = {}", e.what(), id, dat );
        return nullptr;
    }

}

std::shared_ptr<UpdateEvent> UpdateEvent::deserialize( const std::string & serialized ) {

    if ( serialized.substr( 0, HEADER.size() ) != HEADER ) {
        throw std::invalid_argument( "Not a valid UpdateEvent serial." );
    }

    std::unordered_map<std::string, std::shared_ptr<Object>> updates;

    size_t start = HEADER.size();
    while ( start < serialized.size() ) {

        size_t end = serialized.find( END_TAG, start );
        if ( end == std::string::npos ) {
            LOGGER->error( "Dropping unterminated entry: {}", serialized.substr( start ) );
            break;
        }
        
        std::string id;
        std::shared_ptr<Object> data = readEntry( serialized, start, end, id );

        if ( data != nullptr ) {
            if ( updates.count( id ) > 0 ) {
                LOGGER->warn( "Duplicate key: {}", id );
            }
            updates[id] = data;
        }

        start = end + 1;

    }

    return std::make_shared<UpdateEvent>( updates );

}

void UpdateEvent::apply( std::shared_ptr<Object> ) const {}