#pragma once
#include <memory>
#include "EventClasses/event.h"
//#include "server.h"

#include <GLFW/glfw3.h>
class EventHandler
{
public:
	int c;
	EventHandler();
	std::shared_ptr<Event> createKeyEvent(int key, int action, std::string id);
	std::shared_ptr<Event> createPressEvent(int key, std::string id);
	std::shared_ptr<Event> createReleaseEvent(int key, std::string id);
	std::shared_ptr<Event> createMouseEvent(glm::vec3 direction, std::string id);
};
