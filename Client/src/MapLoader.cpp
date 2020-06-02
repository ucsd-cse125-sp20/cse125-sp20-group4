#pragma warning(disable:4201)

#include "MapLoader.h"

/* camera
 *    |
 *    v
 * +--------------------------+
 * |00000000000000000000000000|
 * |00=--------------------=00|
 * |00000000000000000000000000|
 * |00=--------------------=00|
 * |00000000000000000000000000|
 * +--------------------------+
*/


MapLoader::MapLoader(const std::string mapFile) {
	
    std::ifstream input(mapFile);

    LoadedModel * model;
    glm::vec3 position;
    glm::vec3 direction;
    float scale;

    entities.push_back(new Entity("floor", new RectangularCuboid(glm::vec3(1.0f), 1000.0f, 1.0f, 1000.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    int i = 0;

    int z = 0;
    for (std::string line; getline(input, line); z++)
    {

        int x = 0;
        for ( auto it = line.begin(); it < line.end(); it++, x++)
        {
            position = glm::vec3(x * 2, 0, z * 2);
            scale = 1.0f;
            switch (*it) {
                case '+':
                    model = new LoadedModel("Models/corner_shelf.dae", Shaders::phong());
                    if (x == 0 && z == 0) {
                        direction = glm::vec3(-1.0f, 0.0f, 0.0f); //close right
                    }
                    else if (x != 0 && z == 0) {
                        direction = glm::vec3(0.0f, 0.0f, -1.0f); //close left
                    }
                    else if (x == 0 && z != 0) {
                        direction = glm::vec3(0.0f, 0.0f, 1.0f); //far right
                    }
                    else {
                        direction = glm::vec3(1.0f, 0.0f, 0.0f); //far left
                    }
                    break;
                case '-':
                    model = new LoadedModel("Models/edge_shelf.dae", Shaders::phong());
                    if (z == 0) {
                        direction = glm::vec3(-1.0f, 0.0f, 0.0f);
                    }
                    else {
                        direction = glm::vec3(1.0f, 0.0f, 0.0f);
                    }
                    break;
                case '|':
                    model = new LoadedModel("Models/edge_shelf.dae", Shaders::phong());
                    if (x == 0) {
                        direction = glm::vec3(0.0f, 0.0f, 1.0f);
                    }
                    else {
                        direction = glm::vec3(0.0f, 0.0f, -1.0f);
                    }
                    break;
                case '=':
                    model = new LoadedModel("Models/shopper.dae", Shaders::phong());
                    direction = glm::vec3(1.0f, 0.0f, 0.0f);
                    scale = 0.1f;
                    break;
                case '1':
                    model = new LoadedModel("Models/shopper.dae", Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 0, 0));
                    direction = glm::vec3(1.0f, 0.0f, 0.0f);
                    scale = 0.1f;
                    break;
                case '2':
                    model = new LoadedModel("Models/shopper.dae", Shaders::phong());
                    model->setColor(glm::vec3(0, 1.0f, 0));
                    direction = glm::vec3(1.0f, 0.0f, 0.0f);
                    scale = 0.1f;
                    break;
                case '3':
                    model = new LoadedModel("Models/shopper.dae", Shaders::phong());
                    model->setColor(glm::vec3(0, 0, 1.0f));
                    direction = glm::vec3(1.0f, 0.0f, 0.0f);
                    scale = 0.1f;
                    break;
                case '4':
                    model = new LoadedModel("Models/shopper.dae", Shaders::phong());
                    model->setColor(glm::vec3(1.0f, 1.0f, 0));
                    direction = glm::vec3(1.0f, 0.0f, 0.0f);
                    scale = 0.1f;
                    break;
                default:
                    continue;
            }
            entities.push_back(new Entity("entity" + std::to_string(i), (model), position, direction, scale));
            i++;
        }
    }

}

std::vector<Entity*> MapLoader::getEntities() {

    return entities;

}