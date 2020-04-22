#ifndef RECTANGULAR_CUBOID_H
#define RECTANGULAR_CUBOID_H

#include "drawing/model/Geometry.h"

class RectangularCuboid : public Geometry {

    public:
    RectangularCuboid( const glm::vec3 & color, float side );
    RectangularCuboid( const glm::vec3 & color, float height, float width, float depth );

};

#endif // !RECTANGULAR_CUBOID_H