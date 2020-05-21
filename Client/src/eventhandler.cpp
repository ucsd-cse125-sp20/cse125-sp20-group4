#include "eventhandler.h"
#include "EventClasses/Movement/moveforwardevent.h"
#include "EventClasses/Movement/movebackwardevent.h"
#include "EventClasses/Movement/moverightevent.h"
#include "EventClasses/Movement/moveleftevent.h"
#include "EventClasses/Movement/stopforwardevent.h"
#include "EventClasses/Movement/stopbackwardevent.h"
#include "EventClasses/Movement/stoprightevent.h"
#include "EventClasses/Movement/stopleftevent.h"
#include "EventClasses/Movement/mouseevent.h"

EventHandler::EventHandler(){
    this->c = 0;
}

std::shared_ptr<Event> EventHandler::createKeyEvent(int key, int action, std::string id) {
    if (action == GLFW_PRESS) {
        return createPressEvent(key, id);
    } else if (action == GLFW_RELEASE) {
        return createReleaseEvent(key, id);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Event> EventHandler::createPressEvent(int key, std::string id){
    std::shared_ptr<Event> event = nullptr;
    switch (key) {
        case GLFW_KEY_W:
            event = std::shared_ptr<MoveForwardEvent>(new MoveForwardEvent(id));
            break;
        case GLFW_KEY_A:
            event = std::shared_ptr<MoveLeftEvent>(new MoveLeftEvent(id));
            break;
        case GLFW_KEY_S:
            event = std::shared_ptr<MoveBackwardEvent>(new MoveBackwardEvent(id));
            break;
        case GLFW_KEY_D:
            event = std::shared_ptr<MoveRightEvent>(new MoveRightEvent(id));
            break;
    }
    return event;
}

std::shared_ptr<Event> EventHandler::createReleaseEvent(int key, std::string id) {
    std::shared_ptr<Event> event = nullptr;
    switch (key) {
        case GLFW_KEY_W:
            event = std::shared_ptr<StopForwardEvent>(new StopForwardEvent(id));
            break;
        case GLFW_KEY_A:
            event = std::shared_ptr<StopLeftEvent>(new StopLeftEvent(id));
            break;
        case GLFW_KEY_S:
            event = std::shared_ptr<StopBackwardEvent>(new StopBackwardEvent(id));
            break;
        case GLFW_KEY_D:
            event = std::shared_ptr<StopRightEvent>(new StopRightEvent(id));
            break;
    }
    return event;
}

std::shared_ptr<Event> EventHandler::createMouseEvent(std::string id, float angle, glm::vec3 axis) {
    std::shared_ptr<Event> event =  std::shared_ptr<MouseEvent>(new MouseEvent(id,angle, axis));
    return event;
}