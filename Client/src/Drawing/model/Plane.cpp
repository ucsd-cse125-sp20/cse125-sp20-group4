#include "drawing/model/Plane.h"

Plane::Plane(Texture* texture, glm::vec3 bottomLeft, glm::vec3 topRight) : Geometry(texture, Shaders::flat(), GL_TRIANGLES) {

    glm::vec3 bottomRight = glm::vec3(0.0f);
    glm::vec3 topLeft = glm::vec3(0.0f);
    if (abs(topRight.y - bottomLeft.y) <= 0.01f) {
        bottomRight = glm::vec3(topRight.x, bottomLeft.y, bottomLeft.z);
        topLeft = glm::vec3(bottomLeft.x, topRight.y, topRight.z);
    }
    else {
        bottomRight = glm::vec3(topRight.x, bottomLeft.y, topRight.z);
        topLeft = glm::vec3(bottomLeft.x, topRight.y, bottomLeft.z);
    }

    std::vector<glm::vec3> vertices = { 
        bottomLeft, 
        bottomRight, 
        topLeft, 
        topRight 
    };
    std::vector<glm::vec3> colors = { 
        glm::vec3(1.0f), 
        glm::vec3(1.0f), 
        glm::vec3(1.0f), 
        glm::vec3(1.0f) 
    };
    glm::vec3 normal = glm::normalize(glm::cross(topRight - topLeft, topRight - bottomRight));
    std::vector<glm::vec3> normals = { 
        normal, 
        normal, 
        normal, 
        normal 
    };
    std::vector<glm::vec3> texCoords = { 
        glm::vec3(0.0f), 
        glm::vec3(abs(topRight.x - bottomLeft.x) + abs(topRight.z - bottomLeft.z), 0.0f, 0.0f), 
        glm::vec3(0.0f, abs(topRight.y - bottomLeft.y) + abs(topRight.z - bottomLeft.z), 0.0f),
        glm::vec3(abs(topRight.x - bottomLeft.x) + abs(topRight.z - bottomLeft.z), abs(topRight.y - bottomLeft.y) + abs(topRight.z - bottomLeft.z), 0.0f)
    };

    /* Define drawing order */

    std::vector<unsigned int> idx;

    idx.push_back(0);
    idx.push_back(2);
    idx.push_back(1);

    idx.push_back(1);
    idx.push_back(2);
    idx.push_back(3);

    initialize(vertices, colors, normals, texCoords, idx);

}

