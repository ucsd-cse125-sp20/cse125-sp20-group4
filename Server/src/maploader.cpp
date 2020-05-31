#include "maploader.h"
#include "logger.h"
#include "ObjectClasses/Factories/factories.h"
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
            std::shared_ptr<Object> obj;
            switch (*it) {
            case 's':
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z);
                std::dynamic_pointer_cast<Shelf>(obj)->setItem(std::make_shared<RedItemFactory>());
                gs->createObject(obj);
                break;
            case 'w':
                //gs->createObject(std::make_shared<Wall>("ignore", position.x, position.y, position.z));
                break;
            case '0':
                gs->createObject(std::make_shared<Player>("client-0", position.x, position.y, position.z), "client-0");
                break;
            case '1':
                gs->createObject(std::make_shared<Player>("client-1", position.x, position.y, position.z), "client-1");
                break;
            case '2':
                gs->createObject(std::make_shared<Player>("client-2", position.x, position.y, position.z), "client-2");
                break;
            case '3':
                gs->createObject(std::make_shared<Player>("client-3", position.x, position.y, position.z), "client-3");
                break;
            }
            i++;
        }
    }
    log->info("Done Loading map");

}