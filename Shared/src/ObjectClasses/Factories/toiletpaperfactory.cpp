#include "ObjectClasses/Factories/toiletpaperfactory.h"
#include "ObjectClasses/toiletpaper.h"
#include "deserializer.h"
#include "logger.h"

std::shared_ptr<Object> ToiletPaperFactory::create(std::string serial) { //TODO:Make more robust to serialization errors
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

    std::shared_ptr<ToiletPaper> shelf = std::make_shared<ToiletPaper>(id, posx, posy, posz, orx, ory, orz, length, width, height);
    return shelf;

}

std::shared_ptr<Object> ToiletPaperFactory::create() {
    return std::make_shared<ToiletPaper>();
}