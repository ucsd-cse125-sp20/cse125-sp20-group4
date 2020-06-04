#include "ObjectClasses\toiletpaper.h"

ToiletPaper::ToiletPaper() : Object("ignore"){}

ToiletPaper::ToiletPaper(const ToiletPaper& obj) : ToiletPaper(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getOrientationX(), obj.getOrientationY(), obj.getOrientationZ(), obj.getWidth(), obj.getHeight(), obj.getLength()){}

ToiletPaper::ToiletPaper(const std::string& id, float xPos, float yPos, float zPos, float orx, float ory, float orz, float width, float height, float length) : Object(id,xPos,yPos,zPos,orx,ory,orz,width,height,length){}

const std::string& ToiletPaper::getTag()
{
    return TAG;
}

std::string ToiletPaper::serialize() const
{
    return "ToiletPaper:" + Object::serialize();
}

std::shared_ptr<Object> ToiletPaper::clone() const
{
    return std::make_shared<ToiletPaper>(this->getId(), this->getPositionX(), this->getPositionY(), this->getPositionZ(), this->getOrientationX(), this->getOrientationY(), this->getOrientationZ(), this->getWidth(), this->getHeight(), this->getLength());
}