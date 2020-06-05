#ifndef RECTANGULAR_CUBOID_H
#define RECTANGULAR_CUBOID_H

#include "drawing/model/Geometry.h"

class RectangularCuboid : public Geometry {

    public:
    RectangularCuboid( Texture* texture, const glm::vec3 & color, float side );
    RectangularCuboid( Texture* texture, const glm::vec3 & color, float width, float height, float depth );

};

#endif // !RECTANGULAR_CUBOID_H