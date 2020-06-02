#ifndef SANITIZER_H
#define SANITIZER_H

#include <vector>

/**
 * Adds a mapping between character ch and sequence seq to vector vec
 */
#define ADD_CHAR_MAPPING( vec, ch, seq ) ( ( vec ).push_back( std::make_pair( ( ch ), ( seq ) ) ) ) 

/**
 * Sanitizes strings, that is, removes the occurrence of certain special characters from the string
 * in a way that is recoverable later.
 *
 * The expressions that characters are replaced by are preceded by MARKER_CHAR and succeded by
 * TRAIL_CHAR. As such, both those characters are already special characters and may not be
 * used. Moreover, MARKER_EXP is already in use and may not be used again.
 */
class Sanitizer {

    public:
    using CharMapping = std::pair<char, std::string>;

    static constexpr char MARKER_CHAR = '&';
    inline static const std::string MARKER_EXP = "amp";

    // Marks the end of an expression. Isn't replaced, but is reserved
    static constexpr char TRAIL_CHAR = ';';

    /**
     * Initializes a sanitizer that uses the given character-to-expression mapping.
     *
     * @param specialCharacters The special characters to use and the expressions to
     *                          map them to.
     * @throws std::invalid_argument if a character character or expression appears in
     *                               more than one mapping, or if a reserved character or
     *                               expression appears in a mapping.
     */
    Sanitizer( std::vector<CharMapping> specialCharacters );

    /**
     * Sanitizes a message.
     *
     * @param message The message to sanitize.
     * @return The sanitized message.
     */
    std::string sanitize( const std::string & message ) const;
    /**
     * Restores the original string from its sanitized version.
     *
     * @param sanitized The sanitized message.
     * @return The original message.
     */
    std::string restore( const std::string & sanitized ) const;

    protected:
    using Mapping = std::pair<std::string, std::string>;

    /**
     * Replaces all instances of seq in s by repl, placing the result in dest.
     */
    static std::string replaceAll( const std::string & s, const std::string & seq, const std::string & repl );
    /**
     * Parses mappings to internal format and checks for illegal mappings.
     */
    static std::vector<Mapping> parseMappings( const std::vector<CharMapping> & raw );

    const std::vector<Mapping> specialCharacters;

    static_assert( MARKER_CHAR != TRAIL_CHAR, "Marker and trailing chars cannot be the same." );

};

#endif