#include <limits>
#include <vector>

#include <glm/mat4x4.hpp>

#include "drawing/Shaders.h"
#include "drawing/model/Axis.h"

#define MAX_COORD 1E+2f
#define MIN_COORD -MAX_COORD
#define ARROW_OFFSET 10.0f
#define ARROW_SIZE 1.0f

#define POINT_SIZE 3.0f
#define WHITE glm::vec3( 1.0f, 1.0f, 1.0f )
#define BLACK glm::vec3( 0.0f, 0.0f, 0.0f )
#define RED glm::vec3( 1.0f, 0.0f, 0.0f )
#define DARK_RED glm::vec3( 0.01f, 0.0f, 0.0f )
#define GREEN glm::vec3( 0.0f, 1.0f, 0.0f )
#define DARK_GREEN glm::vec3( 0.0f, 0.01f, 0.0f )
#define BLUE glm::vec3( 0.0f, 0.0f, 1.0f )
#define DARK_BLUE glm::vec3( 0.0f, 0.0f, 0.01f )

static void setAxisColor( std::vector<glm::vec3> & colors, const glm::vec3 & color, const unsigned int labelVertexCount ) {

    colors.push_back( WHITE );
    colors.push_back( color );
    colors.push_back( color );
    colors.push_back( BLACK );

    for ( unsigned int i = 0; i < labelVertexCount; i++ ) {

        colors.push_back( color ); // Push vertex color for each vertex used for drawing the axis label.

    }

}

/* Constructor */

Axis::Axis( bool enabled, float scale ) : Geometry( NULL, Shaders::flat(), GL_LINES ), normalPointSize( 0.0f ), enabled( enabled ) {

    const float max_coord = MAX_COORD * scale;
    const float min_coord = MIN_COORD * scale;
    const float arrowOffset = ARROW_OFFSET * scale;
    const float arrowSize = ARROW_SIZE * scale;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> idx;

    // X axis
    vertices.push_back( glm::vec3( max_coord, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( min_coord, 0.0f, 0.0f ) );

    // Arrow for X axis
    vertices.push_back( glm::vec3( arrowOffset, arrowSize, arrowSize ) );
    vertices.push_back( glm::vec3( arrowOffset, -arrowSize, -arrowSize ) );
    vertices.push_back( glm::vec3( arrowOffset, arrowSize, -arrowSize ) );
    vertices.push_back( glm::vec3( arrowOffset, -arrowSize, arrowSize ) );

    setAxisColor( colors, RED, 4 ); // Set X axis color.

    // Y axis
    vertices.push_back( glm::vec3( 0.0f, max_coord, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, min_coord, 0.0f ) );

    // Arrow for Y axis
    vertices.push_back( glm::vec3( -arrowSize, arrowOffset, arrowSize ) );
    vertices.push_back( glm::vec3( 0.0f, arrowOffset, 0.0f ) );
    vertices.push_back( glm::vec3( arrowSize, arrowOffset, arrowSize ) );
    vertices.push_back( glm::vec3( 0.0f, arrowOffset, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, arrowOffset, -arrowSize ) );
    vertices.push_back( glm::vec3( 0.0f, arrowOffset, 0.0f ) );

    setAxisColor( colors, GREEN, 6 ); // Set Y axis color.

    // Z axis
    vertices.push_back( glm::vec3( 0.0f, 0.0f, max_coord ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    vertices.push_back( glm::vec3( 0.0f, 0.0f, min_coord ) );

    // Arrow for Z axis
    vertices.push_back( glm::vec3( -arrowSize, arrowSize, arrowOffset ) );
    vertices.push_back( glm::vec3( arrowSize, arrowSize, arrowOffset ) );
    vertices.push_back( glm::vec3( arrowSize, arrowSize, arrowOffset ) );
    vertices.push_back( glm::vec3( -arrowSize, -arrowSize, arrowOffset ) );
    vertices.push_back( glm::vec3( -arrowSize, -arrowSize, arrowOffset ) );
    vertices.push_back( glm::vec3( arrowSize, -arrowSize, arrowOffset ) );

    setAxisColor( colors, BLUE, 6 ); // Set Z axis color.

    for ( unsigned int i = 0; i < vertices.size(); i++ ) {

        normals.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
        idx.push_back( i );

    }

    initialize( vertices, colors, normals, vertices, idx );

}

/* Public methods */

bool Axis::enabledAll = false;

void Axis::setEnabledAll( bool en ) {

    enabledAll = en;

}

void Axis::toggleAll() {

    setEnabledAll( !enabledAll );

}

void Axis::setEnabled( bool en ) {

    enabled = en;

}

void Axis::toggle() {

    setEnabled( !enabled );

}

void Axis::draw( const glm::mat4x4 & model, const glm::mat4x4 & view, const glm::vec3 & direction ) const {

    if ( enabledAll && enabled ) { // Only draw if axes are enabled
        Geometry::draw( model, view, direction );
    }

}

/* Protected methods */

void Axis::setup() const {

    glGetFloatv( GL_POINT_SIZE, &normalPointSize );
    glLineWidth( POINT_SIZE );

}

void Axis::restore() const {

    glLineWidth( normalPointSize );

}