#include "drawing/Shaders.h"
#include "drawing/model/RectangularCuboid.h"

RectangularCuboid::RectangularCuboid( const glm::vec3 & color, float side ) : RectangularCuboid( color, side, side, side ) {}

RectangularCuboid::RectangularCuboid( const glm::vec3 & color, float height, float width, float depth ) : Geometry( Shaders::flat(), GL_TRIANGLES ) {

    height /= 2;
    width /= 2;
    depth /= 2;

    /* Calculate vertices and normals */

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;

    for ( int i = -1; i <= 1; i += 2 ) {
        for ( int j = -1; j <= 1; j += 2 ) {
            for ( int k = -1; k <= 1; k += 2 ) {
                vertices.push_back( glm::vec3( i * width, j * height, k * depth ) );
                colors.push_back( color );
                normals.push_back( glm::normalize( glm::vec3( ( float ) i, ( float ) j, ( float ) k ) ) );
            }
        }
    }

    /* Define drawing order */

    std::vector<unsigned int> idx;

    // Left face
    idx.push_back( 0 );
    idx.push_back( 2 );
    idx.push_back( 1 );

    idx.push_back( 1 );
    idx.push_back( 2 );
    idx.push_back( 3 );

    // Right face
    idx.push_back( 5 );
    idx.push_back( 7 );
    idx.push_back( 4 );

    idx.push_back( 4 );
    idx.push_back( 7 );
    idx.push_back( 6 );

    // Back face
    idx.push_back( 0 );
    idx.push_back( 4 );
    idx.push_back( 2 );

    idx.push_back( 2 );
    idx.push_back( 4 );
    idx.push_back( 6 );

    // Front face
    idx.push_back( 1 );
    idx.push_back( 3 );
    idx.push_back( 5 );

    idx.push_back( 3 );
    idx.push_back( 7 );
    idx.push_back( 5 );

    // Bottom face
    idx.push_back( 0 );
    idx.push_back( 1 );
    idx.push_back( 5 );

    idx.push_back( 0 );
    idx.push_back( 5 );
    idx.push_back( 4 );

    // Top Face
    idx.push_back( 2 );
    idx.push_back( 6 );
    idx.push_back( 3 );

    idx.push_back( 3 );
    idx.push_back( 6 );
    idx.push_back( 7 );

    initialize( vertices, colors, normals, idx );

}