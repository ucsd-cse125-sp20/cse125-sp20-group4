#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include "state/World.h"

class Window {

    private:

    static glm::vec3 trackBallMapping( float x, float y );
    static void rotateCamera( float angle, glm::vec3 axis );

    public:

    static int width;
    static int height;
    static glm::mat4 P; // P for projection
    static glm::mat4 V; // V for view
    static glm::vec3 cam_pos, cam_dir; // Where the camera is and points to.

    static float nearZ;
    static float farZ;
    static float fov;
    static float aspect;

    static GLFWwindow * window;
    static World world;

    static void setCamera();
    static void initialize();
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