#include "ObjectClasses/Factories/shelffactory.h"
#include "deserializer.h"

std::shared_ptr<Object> ShelfFactory::create(std::string serial) { //TODO:Make more robust to serialization errors
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
    //float orx = std::stof(serial.substr(last, pos - last)); //orx

    last = pos + 1;
    pos = serial.find(",", last);
    //float ory = std::stof(serial.substr(last, pos - last)); //ory

    last = pos + 1;
    pos = serial.find(",", last);
    //float orz = std::stof(serial.substr(last, pos - last)); //orz

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

    pos = serial.find("/", last);
    /*
    //handle this differently
    Deserializer deserializer;
    std::string heldString = serial.substr(last, pos - last);
    std::shared_ptr<Object> heldObj = nullptr;
    if (heldString != "noItem") {
        heldObj = deserializer.deserializeObject(heldString);
    }
    */
    std::shared_ptr<Shelf> shelf = std::make_shared<Shelf>(id, posx, posy, posz, length, width, height, nullptr);
    return shelf;

}

std::shared_ptr<Object> ShelfFactory::create() {
    return std::make_shared<Shelf>();
}