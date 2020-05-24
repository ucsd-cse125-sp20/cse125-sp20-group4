#include <sstream>
#include <stdexcept>
#include <unordered_set>

#include "util/Sanitizer.h"

/* Constructor */

Sanitizer::Sanitizer( std::vector<CharMapping> specialCharacters ) : specialCharacters( parseMappings( specialCharacters ) ) {}

/* Public functions */

std::string Sanitizer::sanitize( const std::string & message ) const {

    std::string s = message;
    for ( auto it = specialCharacters.cbegin(); it != specialCharacters.cend(); it++ ) {

        // Replace each special character in the string
        s = replaceAll( s, it->first, it->second );

    }
    return s;

}

std::string Sanitizer::restore( const std::string & sanitized ) const {

    std::string s = sanitized;
    for ( auto it = specialCharacters.crbegin(); it != specialCharacters.crend(); it++ ) {

        // Restore each special character in the string in reverse order
        s = replaceAll( s, it->second, it->first );

    }
    return s;

}

/* Helpers */

std::string Sanitizer::replaceAll( const std::string & s, const std::string & seq, const std::string & repl ) {

    std::ostringstream str;
    size_t pos;
    size_t lastPos = 0;
    while ( ( pos = s.find( seq, lastPos ) ) != std::string::npos ) {

        str << s.substr( lastPos, pos - lastPos ) << repl;
        lastPos = pos + seq.size();

    }
    str << s.substr( lastPos );
    return str.str();

}

#define CHAR_TO_STR( c ) std::string( 1, ( c ) )
#define MAKE_EXPR( expr ) ( MARKER_CHAR + ( expr ) + TRAIL_CHAR )

std::vector<Sanitizer::Mapping> Sanitizer::parseMappings( const std::vector<CharMapping> & raw ) {

    std::vector<Mapping> parsed;
    std::unordered_set<char> usedChars;
    std::unordered_set<std::string> usedExprs;

    // Always replace the marker character first and restore it last
    ADD_CHAR_MAPPING( parsed, CHAR_TO_STR( MARKER_CHAR ), MAKE_EXPR( MARKER_EXP ) );

    for ( CharMapping m : raw ) {

        // Check for reserved characters/expressions
        if ( m.first == MARKER_CHAR || m.first == TRAIL_CHAR ) {
            throw std::invalid_argument( std::string( "Character '" ) + MARKER_CHAR + std::string( "' is reserved." ) );
        }
        if ( m.second == MARKER_EXP ) {
            throw std::invalid_argument( std::string( "Expression '" ) + MARKER_EXP + std::string( "' is reserved." ) );
        }

        // Check for repeated characters/expressions
        if ( usedChars.count( m.first ) > 0 ) {
            throw std::invalid_argument( std::string( "Duplicate character '" ) + m.first + std::string( "'" ) );
        }
        if ( usedExprs.count( m.second ) > 0 ) {
            throw std::invalid_argument( std::string( "Duplicate expression '" ) + m.second + std::string( "'" ) );
        }

        ADD_CHAR_MAPPING( parsed, CHAR_TO_STR( m.first ), MAKE_EXPR( m.second ) );
        usedChars.insert( m.first );
        usedExprs.insert( m.second );

    }

    return parsed;

}