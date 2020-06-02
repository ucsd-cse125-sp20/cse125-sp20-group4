#include "drawing/Model.h"

void Model::draw( const glm::mat4x4 & model, const glm::mat4x4 & view ) const {

    // Default direction
    static const glm::vec3 direction( 0.0f, 0.0f, 1.0f );
    draw( model, view, direction );

}