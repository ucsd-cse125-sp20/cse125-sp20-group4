#pragma once
#include "object.h"
#include <memory>

class ToiletPaper : public Object {
public:
	ToiletPaper();
	ToiletPaper(const ToiletPaper& obj);
	ToiletPaper(const std::string& id, float xPos, float yPos, float zPos, float orx, float ory, float orz, float width = 1.0f, float height = 1.0f, float length = 1.0f);

	const std::string TAG = "ToiletPaper";
	const std::string& getTag();
	//(de)serialize functions
	virtual std::string serialize() const;
	virtual std::shared_ptr<Object> clone() const override;
};