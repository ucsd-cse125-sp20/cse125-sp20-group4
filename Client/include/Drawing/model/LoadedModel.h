#ifndef LOADED_MODEL_H
#define LOADED_MODEL_H

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "drawing/model/Geometry.h"

class LoadedModel : public Geometry {

private:
    glm::vec3 color = glm::vec3(1.0f);

    void processNode(aiNode *node, const aiScene *scene);
    void processMesh(aiMesh* mesh);
    
public:
    LoadedModel(const std::string file, Texture * texture, const Shader& shaderProgram);

    void draw(const glm::mat4x4& model, const glm::mat4x4& view, const glm::vec3& direction) const;

    void setColor(glm::vec3 color);
};

#endif // !RECTANGULAR_CUBOID_H
