#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include <EventClasses/event.h>

#include "Server.h"
#include "state/Camera.h"
#include "state/World.h"

class Window {

    private:

    static glm::vec3 trackBallMapping( float x, float y );
    static void rotateCamera( float angle, glm::vec3 axis );
    static void handleEvent( const std::shared_ptr<Event> & e );

    public:

    static int width;
    static int height;

    static GLFWwindow * window;
    static World * world;
    static Camera * cam;
    static Server * server;
    static std::string playerName;

    static void initialize(Server* server);
    static void clean_up();
    static GLFWwindow * create_window( int width, int height );
    static void resize_callback( GLFWwindow * window, int width, int height );
    static void idle_callback();
    static void display_callback( GLFWwindow * );
    static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );
    static void mouse_move_callback( GLFWwindow * window, double xpos, double ypos );
    static void mouse_button_callback( GLFWwindow * window, int button, int action, int mods );
    static void mouse_scroll_callback( GLFWwindow * window, double xoffset, double yoffset );

};

#endif