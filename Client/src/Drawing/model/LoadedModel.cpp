#pragma warning(disable:4201)

#include <stdexcept>

#include "drawing/model/LoadedModel.h"

LoadedModel::LoadedModel(const std::string file, Texture * texture, const Shader& shaderProgram) : Geometry( texture, shaderProgram, GL_TRIANGLES)
{

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error( "Could not load model." );
    }

    processNode(scene->mRootNode, scene);
}

void LoadedModel::processNode(aiNode *node, const aiScene *scene) {
    
    //aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
    //processMesh(mesh);
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void LoadedModel::processMesh(aiMesh *mesh) {
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> texCoords;
    std::vector<unsigned int> faces;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertices.push_back(vec);

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        normals.push_back(normal);

        glm::vec3 texCoord;
        texCoord.x = mesh->mTextureCoords[0][i].x;
        texCoord.y = mesh->mTextureCoords[0][i].y;
        texCoord.z = mesh->mTextureCoords[0][i].z;
        texCoords.push_back(texCoord);

        colors.push_back(glm::vec3(1.0f));
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            faces.push_back(face.mIndices[j]);
        }
    }

    initialize(vertices, colors, normals, texCoords, faces);

}

void LoadedModel::draw(const glm::mat4x4& model, const glm::mat4x4& view, const glm::vec3& direction) const{
    
    glUseProgram(shaderProgram);

    GLuint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint ambientLoc = glGetUniformLocation(shaderProgram, "ambientCoefficient");
    GLuint dirLightColorLoc = glGetUniformLocation(shaderProgram, "dirLight.color");
    GLuint dirLightDirLoc = glGetUniformLocation(shaderProgram, "dirLight.direction");
    GLuint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
    GLuint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
    GLuint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
    GLuint matShininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");
    GLuint matOpacityLoc = glGetUniformLocation(shaderProgram, "material.opacity");
    GLuint dOnLoc = glGetUniformLocation(shaderProgram, "directionalOn");
    GLuint shadowsOnLoc = glGetUniformLocation(shaderProgram, "shadowsOn");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    glm::vec3 cameraPos = glm::vec3(view[3]);
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
    float ambientCoefficient = 0.5f;
    glUniform1fv(ambientLoc, 1, &ambientCoefficient);
    glUniform3fv(dirLightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform3fv(dirLightDirLoc, 1, glm::value_ptr(glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f))));
    glUniform3fv(matAmbientLoc, 1, glm::value_ptr(glm::vec3(0.8f)));
    glUniform3fv(matDiffuseLoc, 1, glm::value_ptr(glm::vec3(0.2f)));
    glUniform3fv(matSpecularLoc, 1, glm::value_ptr(glm::vec3(0.0f)));
    float shininess = 0, opacity = 1.0f;
    glUniform1fv(matShininessLoc, 1, &shininess);
    glUniform1fv(matOpacityLoc, 1, &opacity);
    int dOn = 1, sOn = 0;
    glUniform1iv(dOnLoc, 1, &dOn);
    glUniform1iv(shadowsOnLoc, 1, &sOn);
    
    //Geometry::draw(model, view, direction);
    Geometry::draw( model, view, glm::normalize( glm::vec3( direction.x, 0.0f, direction.z ) ) );

}

void LoadedModel::setColor(glm::vec3 c) {

    color = c;

}