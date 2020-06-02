#include "ObjectClasses/Factories/shelffactory.h"
#include "ObjectClasses/Factories/useablefactory.h"
#include "ObjectClasses/Factories/barricadefactory.h"
#include "deserializer.h"
#include "logger.h"

std::shared_ptr<Object> ShelfFactory::create(std::string serial) { //TODO:Make more robust to serialization errors
    auto log = getLogger("ShelfFactory");
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);

    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last); //id

    last = pos + 1;
    pos = serial.find(",", last);
    float posx = std::stof(serial.substr(last, pos - last)); //x

    last = pos + 1;
    pos = serial.find(",", last);
    float posy = std::stof(serial.substr(last, pos - last)); //y

    last = pos + 1;
    pos = serial.find(",", last);
    float posz = std::stof(serial.substr(last, pos - last)); //z

    last = pos + 1;
    pos = serial.find(",", last);
    float orx = std::stof(serial.substr(last, pos - last)); //orx

    last = pos + 1;
    pos = serial.find(",", last);
    float ory = std::stof(serial.substr(last, pos - last)); //ory

    last = pos + 1;
    pos = serial.find(",", last);
    float orz = std::stof(serial.substr(last, pos - last)); //orz

    last = pos + 1;
    pos = serial.find(",", last);
    float width = std::stof(serial.substr(last, pos - last)); //width

    last = pos + 1;
    pos = serial.find(",", last);
    float height = std::stof(serial.substr(last, pos - last)); //height

    last = pos + 1;
    pos = serial.find(",", last);
    float length = std::stof(serial.substr(last, pos - last)); //length

    last = pos + 1;
    pos = serial.find(",", last);
    std::string tmp = serial.substr(last, pos - last);
    int isCorner = std::stoi(tmp); //isCorner
    last = pos + 1;

    pos = serial.find("/", last);
    
    //handle this differently
    std::string heldString = serial.substr(last, pos - last);
    std::shared_ptr<IObjectFactory> heldObj = nullptr;
    if (heldString.compare("Red")==0) {
        heldObj = std::make_shared<RedItemFactory>();
    } else if (heldString.compare("Green")==0) {
        heldObj = std::make_shared<GreenItemFactory>();
    } else if (heldString.compare("Blue")==0) {
        heldObj = std::make_shared<BlueItemFactory>();
    } else if (heldString.compare("Barricade") == 0) {
        heldObj = std::make_shared<BarricadeFactory>();
    }
    
    std::shared_ptr<Shelf> shelf = std::make_shared<Shelf>(id, posx, posy, posz, orx, ory, orz, length, width, height, heldObj, isCorner);
    return shelf;

}

std::shared_ptr<Object> ShelfFactory::create() {
    return std::make_shared<Shelf>();
}