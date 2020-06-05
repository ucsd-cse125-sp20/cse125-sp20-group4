#pragma once

#include <cmath>

#include "drawing/Shaders.h"
#include "drawing/model/Geometry.h"

class Plane : public Geometry {

public:
    Plane(Texture* texture, glm::vec3 bottomLeft, glm::vec3 topRight);

};