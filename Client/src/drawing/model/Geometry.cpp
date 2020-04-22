#include <stdexcept>
#include <unordered_set>

#include "glm/gtc/matrix_transform.hpp"

#include "logger.h"
#include "drawing/model/Geometry.h"

static const auto LOGGER = getLogger( "Geometry" );

/* Constructor and destructor */

Geometry::Geometry( const Shader & shaderProgram, GLenum drawMode ) : shaderProgram( shaderProgram ), drawMode( drawMode ), initialized( false ) {

    // Valid draw modes
    static const std::unordered_set<GLenum> DRAW_MODES { GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, 
                                                         GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, 
                                                         GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY, GL_PATCHES };
    if ( DRAW_MODES.count( drawMode ) == 0 ) { // Check that draw mode is valid
        throw std::invalid_argument( "Invalid draw mode." );
    }

    /* Generate buffers */
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

}

Geometry::Geometry( const Shader & shaderProgram, const std::vector<glm::vec3> & vertices, const std::vector<glm::vec3> & colors, const std::vector<glm::vec3> & normals, std::vector<unsigned int> indices, GLenum drawMode ) : Geometry( shaderProgram, drawMode ) {

    initialize( vertices, colors, normals, indices );

}

Geometry::~Geometry() {

    /* Delete previously generated buffers */
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );

}

/* Public functions */
#include "Window.h"
void Geometry::draw( const glm::mat4x4 & toView, const glm::vec3 & direction ) const {

    if ( !initialized ) { // Check that is already initialized
        throw std::runtime_error( "Attempted to draw geometry before initializing it." );
    }

    LOGGER->trace( "Drawing geometry." );

    glUseProgram( shaderProgram );

    // Calculate rotation matrix for direction
    // NOTE: assume direction is a unit vector
    static const glm::vec3 FOWARD( 0.0f, 0.0f, 1.0f );
    static const glm::mat4x4 I( 1.0f );

    float angle = glm::acos( glm::dot( FOWARD, direction ) );
    glm::vec3 axis = glm::cross( FOWARD, direction );
    glm::mat4x4 directionRotate = angle == 0.0f ? I : glm::rotate( I, angle, axis );

    // Send combined projection-view=model matrix to shader programs.
    glm::mat4x4 model = toView * directionRotate;
    glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "model" ), 1, GL_FALSE, &model[0][0] );

    // Bind vertex array.
    glBindVertexArray( VAO );

    // Now bind a VBO to it as a GL_ARRAY_BUFFER (drawing data).
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    // Populate buffer with vertex data.
    glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof( float ), data.data(), GL_STREAM_DRAW );

    // Position.
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, // Attribute ID.
                           3, // How many components there are per vertex.
                           GL_FLOAT, // What type these components are.
                           GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't.
                           9 * sizeof( GLfloat ), // Offset between consecutive indices.
                           ( GLvoid * ) 0 ); // Offset of the first vertex's component.

    // Color.
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, // Attribute ID.
                           3, // How many components there are per vertex.
                           GL_FLOAT, // What type these components are.
                           GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't.
                           9 * sizeof( GLfloat ), // Offset between consecutive indices.
                           ( GLvoid * ) ( 3 * sizeof( GLfloat ) ) ); // Offset of the first vertex's component.

    // Normal.
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, // Attribute ID.
                           3, // How many components there are per vertex.
                           GL_FLOAT, // What type these components are.
                           GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't.
                           9 * sizeof( GLfloat ), // Offset between consecutive indices.
                           ( GLvoid * ) ( 6 * sizeof( GLfloat ) ) ); // Offset of the first vertex's component.

    // Bind GL_ELEMENT_ARRAY_BUFFER for drawing order.
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );

    // Buffer drawing order.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), indices.data(), GL_STREAM_DRAW );

    // Tell OpenGL to draw with triangles, using the recorded amount of indices and no offset.
    glDrawElements( drawMode, ( GLsizei ) indices.size(), GL_UNSIGNED_INT, 0 );

    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers.
    glBindVertexArray( 0 );

    LOGGER->trace( "Finished drawing geometry." );

}

/* Protected functions */

void Geometry::initialize( const std::vector<glm::vec3> & vertices, const std::vector<glm::vec3> & colors, const std::vector<glm::vec3> & normals, std::vector<unsigned int> idx ) {

    if ( vertices.size() != colors.size() || vertices.size() != normals.size() ) {
        throw std::invalid_argument( "Geometry data lengths do not match." );
    }

    LOGGER->debug( "Initializing geometry with {} vertices and {} indices.", vertices.size(), idx.size() );

    /* Ensure geometry data is empty */

    data.clear();

    /* Combines data for each vertex and push to vector */

    for ( unsigned int i = 0; i < vertices.size(); i++ ) { // Combined data for each vertex

        // Position.
        data.push_back( vertices[i].x );
        data.push_back( vertices[i].y );
        data.push_back( vertices[i].z );

        // Color.
        data.push_back( colors[i].x );
        data.push_back( colors[i].y );
        data.push_back( colors[i].z );

        // Normals.
        data.push_back( normals[i].x );
        data.push_back( normals[i].y );
        data.push_back( normals[i].z );

    }

    /* Store drawing indices */

    indices = idx;

    initialized = true;

}