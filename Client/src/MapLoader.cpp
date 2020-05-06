#include "MapLoader.h"

/*
 * +-----+
 * |00000|
 * |0=-=0|
 * |00000|
 * |0=-=0|
 * |00000|
 * +-----+
*/


MapLoader::MapLoader(const std::string mapFile) {
	
    std::ifstream input(mapFile);
    int z = 0;
    for (std::string line; getline(input, line); z++)
    {
        int x = 0;
        for ( auto it = line.begin(); it < line.end(); it++, x++)
        {
            modelFiles.push_back("Models/sphere.obj");
            positions.push_back(glm::vec3(x * 2, 0, z * 2));
            scales.push_back(1.0f);
            switch (*it) {
                case '+':
                    //modelFiles.push_back("corner_shelf.obj");
                    directions.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                case '-':
                    //modelFiles.push_back("shelf.obj");
                    directions.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                case '|':
                    //modelFiles.push_back("shelf.obj");
                    directions.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
                    break;
                case '=':
                    //modelFiles.push_back("end_shelf.obj");
                    directions.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                    break;
                default:
                    modelFiles.pop_back();
                    positions.pop_back();
                    scales.pop_back();
            }
        }
    }

}

std::vector<std::string> MapLoader::getModelFiles() {

    return modelFiles;

}

std::vector<glm::vec3> MapLoader::getPositions() {

    return positions;

}

std::vector<glm::vec3> MapLoader::getDirections() {

    return directions;

}

std::vector<float> MapLoader::getScales() {

    return scales;

}