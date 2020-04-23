#ifndef EMPTY_MODEL_H
#define EMPTY_MODEL_H

class EmptyModel : public Model {

    void draw( const glm::mat4x4 &, const glm::vec3 & ) const {};

};

#endif // !EMPTY_MODEL_H