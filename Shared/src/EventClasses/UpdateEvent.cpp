#include <stdexcept>

#include "deserializer.h"
#include "EventClasses/UpdateEvent.h"
#include "logger.h"
#include "network/Connection.h"

const std::string UpdateEvent::TAG = "UpdateEvent";
static const std::string HEADER = UpdateEvent::TAG + ":";

static const std::string DATA_TAG = "&data;";
static const std::string END_TAG = "&end;";

static const auto LOGGER = getLogger( "UpdateEvent" );

UpdateEvent::UpdateEvent( const std::unordered_map<std::string, std::shared_ptr<Object>> & updates ) : Event( "" ), updates( updates ) {}

std::string UpdateEvent::serialize() const {

    std::string serial = HEADER;
    for ( auto it = updates.cbegin(); it != updates.cend(); it++ ) {

        serial += Connection::sanitize( it->first ) + DATA_TAG + Connection::sanitize( it->second->serialize() ) + END_TAG;

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
    id = Connection::restore( str.substr( start, data - start ) );
    data += DATA_TAG.size();
    std::string dat = Connection::restore( str.substr( data, end - data ) );

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

        start = end + END_TAG.size();

    }

    return std::make_shared<UpdateEvent>( updates );

}

void UpdateEvent::apply( std::shared_ptr<Object> ) const {}