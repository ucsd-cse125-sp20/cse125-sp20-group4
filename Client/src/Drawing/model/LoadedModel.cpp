#include "drawing/model/LoadedModel.h"

LoadedModel::LoadedModel(const std::string file) : Geometry(Shaders::flat(), GL_TRIANGLES)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> faces;

    FILE* objfile;
    fopen_s( &objfile, file.c_str(), "r");
    char type[3];
    float x, y, z;
    float r, g, b;
    unsigned int v1, v2, v3;

    while (fscanf_s(objfile, "%2s", type, 3) != EOF)
    {
        if (strcmp(type, "v") == 0)
        {
            if (fscanf_s(objfile, "%f %f %f%*[^\n]%*[\n]", &x, &y, &z) != EOF) {
                vertices.push_back(glm::vec3(x, y, z));
                colors.push_back(glm::vec3(0.75, 0.75, 0.75));
            }
        }
        else if (strcmp(type, "vn") == 0) {
            if (fscanf_s(objfile, "%f %f %f%*[\n]", &r, &g, &b) != EOF) {
                normals.push_back(glm::vec3(r, g, b));
            }
        }
        else if (strcmp(type, "f") == 0) {
            if (fscanf_s(objfile, "%d//%d %d//%d %d//%d%*[\n]", &v1, &v1, &v2, &v2, &v3, &v3) != EOF) {
                faces.push_back(v1 - 1);
                faces.push_back(v2 - 1);
                faces.push_back(v3 - 1);
            }
        }
    }

    initialize(vertices, colors, normals, faces);

}