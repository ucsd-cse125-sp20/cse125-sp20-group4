#include "maploader.h"
#include "logger.h"
#include "ObjectClasses/Factories/factories.h"
#include "ObjectClasses/spawnpoint.h"
#include "ObjectClasses/toiletpaper.h"
void MapLoader::LoadMap(std::string file, GameState* gs)
{
    auto log = getLogger("MapLoader");
    std::ifstream input(file);

    log->info("Starting Loading map");
    glm::vec3 position;
    //glm::vec3 direction;

    int z = 0;
    for (std::string line; getline(input, line); z++)
    {
        int x = 0;
        for (auto it = line.begin(); it < line.end(); it++, x++)
        {
            position = glm::vec3(x, 0.0f, z);
            std::shared_ptr<Object> obj;
            std::shared_ptr<IObjectFactory> fac = nullptr;
            switch (*it) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
                // shelf facing up
                log->debug("Made an upward shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, 1.0f, 0.0f, 0.0f);
                switch (*it)
                {
                case 'b':
                    fac = std::make_shared<RedItemFactory>();
                    break;
                case 'c':
                    fac = std::make_shared<GreenItemFactory>();
                    break;
                case 'd':
                    fac = std::make_shared<BlueItemFactory>();
                    break;
                case 'e':
                    fac = std::make_shared<BarricadeFactory>();
                    break;
                }
                if (fac != nullptr) {
                    std::dynamic_pointer_cast<Shelf>(obj)->setItem(fac);
                }
                gs->createObject(obj);
                break;
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
                // shelf facing down
                log->debug("Made a down shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, -1.0f, 0.0f, 0.0f);
                switch (*it)
                {
                case 'g':
                    fac = std::make_shared<RedItemFactory>();
                    break;
                case 'h':
                    fac = std::make_shared<GreenItemFactory>();
                    break;
                case 'i':
                    fac = std::make_shared<BlueItemFactory>();
                    break;
                case 'j':
                    fac = std::make_shared<BarricadeFactory>();
                    break;
                }
                if (fac != nullptr) {
                    std::dynamic_pointer_cast<Shelf>(obj)->setItem(fac);
                }
                gs->createObject(obj);
                break;
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
                // shelf facing right
                log->debug("Made a right shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, 0.0f, 0.0f, 1.0f);
                switch (*it)
                {
                case 'l':
                    fac = std::make_shared<RedItemFactory>();
                    break;
                case 'm':
                    fac = std::make_shared<GreenItemFactory>();
                    break;
                case 'n':
                    fac = std::make_shared<BlueItemFactory>();
                    break;
                case 'o':
                    fac = std::make_shared<BarricadeFactory>();
                    break;
                }
                if (fac != nullptr) {
                    std::dynamic_pointer_cast<Shelf>(obj)->setItem(fac);
                }
                gs->createObject(obj);
                break;
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
                // shelf facing left
                log->debug("Made a left shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, 0.0f, 0.0f, -1.0f);
                switch (*it)
                {
                case 'q':
                    fac = std::make_shared<RedItemFactory>();
                    break;
                case 'r':
                    fac = std::make_shared<GreenItemFactory>();
                    break;
                case 's':
                    fac = std::make_shared<BlueItemFactory>();
                    break;
                case 't':
                    fac = std::make_shared<BarricadeFactory>();
                    break;
                }
                if (fac != nullptr) {
                    std::dynamic_pointer_cast<Shelf>(obj)->setItem(fac);
                }
                gs->createObject(obj);
                break;
            case 'u':
                // top left corner shelf
                log->debug("Made a tlc shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, -1.0f, 0.0f, 0.0f);
                std::dynamic_pointer_cast<Shelf>(obj)->isCorner = 1;
                gs->createObject(obj);
                break;
            case 'v':
                // top right corner shelf
                log->debug("Made a trc shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, 0.0f, 0.0f, -1.0f);
                std::dynamic_pointer_cast<Shelf>(obj)->isCorner = 1;
                gs->createObject(obj);
                break;
            case 'w':
                // bottom left corner shelf
                log->debug("Made a blc shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, 0.0f, 0.0f, 1.0f);
                std::dynamic_pointer_cast<Shelf>(obj)->isCorner = 1;
                gs->createObject(obj);
                break;
            case 'x':
                // bottom right corner shelf
                log->debug("Made a brc shelf");
                obj = std::make_shared<Shelf>("ignore", position.x, position.y, position.z, 1.0f, 0.0f, 0.0f);
                std::dynamic_pointer_cast<Shelf>(obj)->isCorner = 1;
                gs->createObject(obj);
                break;
            case 'y':
                // mob spawn
                log->debug("Made a mob spawn");
                obj = std::make_shared<SpawnPoint>("ignore", position.x, position.y, position.z, 0.0f, 0.0f, 1.0f);
                gs->createObject(obj);
                break;
            case 'z':
                // target
                log->debug("Made a target");
                obj = std::make_shared<ToiletPaper>("ignore", position.x, position.y, position.z, 1.0f, 0.0f, 0.0f);
                gs->createObject(obj);
                break;
            case '0':
                gs->createObject(std::make_shared<Player>("client-0", position.x, 0.0f, position.z), "client-0");
                break;
            case '1':
                gs->createObject(std::make_shared<Player>("client-1", position.x, 0.0f, position.z), "client-1");
                break;
            case '2':
                gs->createObject(std::make_shared<Player>("client-2", position.x, 0.0f, position.z), "client-2");
                break;
            case '3':
                gs->createObject(std::make_shared<Player>("client-3", position.x, 0.0f, position.z), "client-3");
                break;
            case '4':
                gs->createObject(std::make_shared<Player>("client-4", position.x, 0.0f, position.z), "client-4");
                break;
            }
        }
    }
    log->info("Done Loading map");

}