#include "maploader.h"
#include "logger.h"
void MapLoader::LoadMap(std::string file, GameState* gs)
{
    auto log = getLogger("MapLoader");
    std::ifstream input(file);

    log->info("Starting Loading map");
    glm::vec3 position;
    //glm::vec3 direction;
    float scale;

    int i = 0;

    int z = 0;
    for (std::string line; getline(input, line); z++)
    {

        int x = 0;
        for (auto it = line.begin(); it < line.end(); it++, x++)
        {
            position = glm::vec3(x * 2, 0, z * 2);
            scale = 1.0f;
            switch (*it) {
            case 's':
                gs->createObject(std::make_shared<Shelf>("ignore", position.x, position.y, position.z));
                break;
            case 'w':
                //gs->createObject(std::make_shared<Wall>("ignore", position.x, position.y, position.z));
                break;
            }
            i++;
        }
    }
    log->info("Done Loading map");

}