#define NOMINMAX // Stop stupid library from defining max() as a macro

#include <algorithm>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include <EventClasses/events.h>

#include "Window.h"
#include "drawing/Shaders.h"
#include "drawing/model/Axis.h"
#include "drawing/model/EmptyModel.h"
#include "drawing/model/RectangularCuboid.h"
#include "state/CameraEntity.h"
#include "state/Entity.h"

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

const char * window_title = "CSE 125 Project";

#define RADIANS( W ) ( W ) * ( glm::pi<float>() / 180.0f )
#define PRINT_VECTOR( V ) V.x << "|" << V.y << "|" << V.z

#define DEFAULT_CAMERA_POS glm::vec3( 0.0f, 0.0f, 0.0f )
#define DEFAULT_CAMERA_DIR glm::vec3( 0.0f, 0.0f, 1.0f )//-glm::normalize( DEFAULT_CAMERA_POS )

#define POINT_SIZE_FACTOR 0.5f
#define MOUSE_ROTATE_SCALE 1.0f
#define ROTATION_THRESHOLD 0.0001f
#define CAMERA_MOVEMENT_SPEED 0.1f
#define DIRECTIONAL_LIGHT_ROTATION_RATE RADIANS( 1.0f )

#define BASE_PAN_SPEED RADIANS( 1.0f )
#define X_DEAD_ZONE ( Window::width / 2 ) * 0.6
#define Y_DEAD_ZONE ( Window::height / 2 ) * 0.6
#define X_MOVE_SIZE ( Window::width / 2 - X_DEAD_ZONE )
#define Y_MOVE_SIZE ( Window::height / 2 - Y_DEAD_ZONE )
#define X_POS( cursorX ) ( ( cursorX ) - Window::width / 2 )
#define Y_POS( cursorY ) ( ( cursorY ) - Window::height / 2 )
#define X_SPEED( xPos ) ( ( ( std::abs( xPos ) - X_DEAD_ZONE ) / X_MOVE_SIZE ) * ( ( xPos ) > 0 ? 1 : -1 ) * BASE_PAN_SPEED )
#define Y_SPEED( yPos ) ( ( ( std::abs( yPos ) - Y_DEAD_ZONE ) / Y_MOVE_SIZE ) * ( ( yPos ) > 0 ? 1 : -1 ) * BASE_PAN_SPEED )

#define SPECTATOR_CAMERA "spectator"

#define ROTATE( direction, angle, axis ) ( glm::rotate( glm::mat4( 1.0f ), ( angle ), ( axis ) ) * glm::vec4( ( direction ), 1.0f ) )

Camera * Window::cam;
World * Window::world;
Server* Window::server;

std::string Window::playerName = "cube4"; 

void Window::rotateCamera( float angle, glm::vec3 axis ) {

    bool isPlayer = cam->name == Window::playerName;
    if ( cam->isFreeCamera() || isPlayer ) {
        glm::vec3 newDir = ROTATE( cam->getDir(), angle, axis );
        cam->rotate( newDir );
        if ( isPlayer ) { // Send player movement to server
            server->pushEvent( std::make_shared<RotateEvent>( playerName, newDir ) );
        }
    }

}

GLFWwindow * Window::window = nullptr;

int Window::width;
int Window::height;

void Window::initialize( Server * ser ) {

    Shaders::initializeShaders();
    world = new World();
    server = ser;
    cam = Camera::addCamera( SPECTATOR_CAMERA, DEFAULT_CAMERA_POS, DEFAULT_CAMERA_DIR ); // Static fallback camera

    world->addEntity( new CameraEntity( playerName, 0.0f, new RectangularCuboid(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f), DEFAULT_CAMERA_POS, DEFAULT_CAMERA_DIR, 1.0f, false ) );
    //world->addEntity( "cube2", new Entity( new RectangularCuboid( glm::vec3( 0.0f, 1.0f, 0.0f ), 1.0f ), glm::vec3( 5.0f ), glm::vec3( 1.0f, 0.25f, 1.0f ) ) );
    //world->addEntity( "cube3", new Entity( new RectangularCuboid( glm::vec3( 1.0f, 0.0f, 1.0f ), 2.0f, 5.0f, 2.0f ), glm::vec3( 10.f, -5.0f, -2.0f ), glm::vec3( 0.70f, -1.0f, 1.0f ) ) );
    //world->addEntity( new Entity( "cube4", new RectangularCuboid( glm::vec3( 1.0f, 1.0f, 1.0f ), 1.0f ), glm::vec3( 0.0f, 0.0f, 3.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) );
    world->addEntity( new Entity( "cube5", new RectangularCuboid(glm::vec3(0.0f, 1.0f, 1.0f), 1.0f), glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    cam = Camera::getCamera( playerName );

    // Debugging entities
    world->addEntity( new Entity( "worldAxis", new Axis(), glm::vec3( 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), 1.0f, true ) );

}

void Window::clean_up() {

    Camera::removeCamera( "spectator" );

    delete( world );
    Shaders::deleteShaders();

}

GLFWwindow * Window::create_window( int windowWidth, int windowHeight ) {

    // Initialize GLFW.
    if ( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return NULL;
    }

    // 4x antialiasing
    glfwWindowHint( GLFW_SAMPLES, 4 );

#ifdef __APPLE__ // Because Apple hates comforming to standards
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

    // Create the GLFW window
#ifdef DEBUG_MODE // If debugging mode is on, set up core profile.
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
#endif
    GLFWwindow * newWindow = glfwCreateWindow( windowWidth, windowHeight, window_title, NULL, NULL );

    // Check if the window could not be created
    if ( !newWindow ) {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window
    glfwMakeContextCurrent( newWindow );

    // Set swap interval to 1
    glfwSwapInterval( 1 );

    // Get the width and height of the framebuffer to properly resize the window
    glfwGetFramebufferSize( newWindow, &windowWidth, &windowHeight );
    // Call the resize callback to make sure things get drawn immediately
    Window::resize_callback( newWindow, windowWidth, windowHeight );

    Window::window = newWindow;
    return newWindow;

}

void Window::resize_callback( GLFWwindow *, int newWidth, int newHeight ) {

#ifdef __APPLE__
    glfwGetFramebufferSize( window, &width, &height ); // In case your Mac has a retina display
#endif
    Window::width = newWidth;
    Window::height = newHeight;

    // Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
    glViewport( 0, 0, newWidth, newHeight );

    if ( newHeight > 0 ) {
        Camera::setAspect( ( float ) newWidth / ( float ) newHeight );
    }

}

static glm::vec3 movement( 0.0f, 0.0f, 0.0f ); // Camera movement direction.

void Window::idle_callback() {

    // Translate camera
    if ( ( movement.x != 0.0f ) || ( movement.y != 0.0f ) || ( movement.z != 0.0f ) ) {
        glm::vec4 move = glm::inverse( cam->getV() ) * glm::vec4( movement, 1.0f );
        cam->move( glm::vec3( move ) ); // Camera is moving.
    }

    // Rotate camera (trackball)
    double cursorX, cursorY;
    glfwGetCursorPos( window, &cursorX, &cursorY );
    cursorX = X_POS( cursorX );
    cursorY = Y_POS( cursorY );
#pragma warning( push )
#pragma warning( disable: 4244 )
    if ( std::abs( cursorX ) > X_DEAD_ZONE ) {
        rotateCamera( X_SPEED( ( float ) cursorX ), glm::vec3( 0.0f, -1.0f, 0.0f ) );
    }
    if ( std::abs( cursorY ) > Y_DEAD_ZONE ) {
        rotateCamera( Y_SPEED( ( float ) cursorY ), glm::cross( glm::vec3( 0.0f, 1.0f, 0.0f ), cam->getDir() ) );
    }
#pragma warning( pop )

}

void Window::display_callback( GLFWwindow * ) {

    // Clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Dunno if actually needed

    // Render scene.
    world->draw( cam->getToView() );

    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers( window );
}

static float pointSize = 1.0f;

void Window::key_callback( GLFWwindow * focusWindow, int key, int, int action, int mods ) {
    
    // Check for a key press
    if ( action == GLFW_PRESS ) {
        // Check if escape was pressed
        switch ( key ) {

            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose( focusWindow, GL_TRUE );
                break;

            case GLFW_KEY_W: // Start moving forward.
                if ( cam->isFreeCamera() ) {
                    movement.z -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<MoveForwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_S: // Start moving backward.
                if ( cam->isFreeCamera() ) {
                    movement.z += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<MoveBackwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_A: // Start moving left.
                if ( cam->isFreeCamera() ) {
                    movement.x -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<MoveLeftEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_D: // Start moving right.
                if ( cam->isFreeCamera() ) {
                    movement.x += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<MoveRightEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_SPACE: // Start moving forward.
                if (cam->name == Window::playerName) {
                    server->pushEvent(std::make_shared<PlaceBarricadeEvent>(playerName));
                }
                break;

            case GLFW_KEY_Q: // Start moving down.
                if ( cam->isFreeCamera() ) {
                    movement.y -= CAMERA_MOVEMENT_SPEED;
                }
                break;

            case GLFW_KEY_E: // Start moving up.
                if ( cam->isFreeCamera() ) {
                    movement.y += CAMERA_MOVEMENT_SPEED;
                }
                break;

            case GLFW_KEY_R: // Reset camera position.
                if ( cam->isFreeCamera() ) {
                    cam->update( DEFAULT_CAMERA_POS, DEFAULT_CAMERA_DIR );
                }
                break;

            case GLFW_KEY_P:
                if ( ( mods & GLFW_MOD_SHIFT ) == 0 ) { // Check for shift key.
                    // No shift - Scale points down. Ensures not smaller than 0.
                    pointSize = std::max( 0.0f, pointSize - POINT_SIZE_FACTOR );
                } else {
                    // Shift - Scale points up.
                    pointSize += POINT_SIZE_FACTOR;
                }
                glPointSize( pointSize );
                break;

            case GLFW_KEY_BACKSLASH: // Print current camera position.
                if ( ( mods & GLFW_MOD_SHIFT ) != 0 ) { // Check for shift key.
                    // std::cout << "Current light direction: " << PRINT_VECTOR( Lights::directionalDirection ) << std::endl;
                } else {
                    std::cout << "Current camera position: " << PRINT_VECTOR( cam->getPos() ) << std::endl;
                    std::cout << "Current camera direction: " << PRINT_VECTOR( cam->getDir() ) << std::endl;
                }
                break;

            case GLFW_KEY_C: // Switch to spectator mode
                cam = Camera::getCamera( cam->name == SPECTATOR_CAMERA ? playerName : SPECTATOR_CAMERA );
                break;

            case GLFW_KEY_X:
                Axis::toggleAll();
                break;

        }
    } else if ( action == GLFW_RELEASE ) { // Check for a key release.
        switch ( key ) {

            case GLFW_KEY_W: // Start moving forward.
                if ( cam->isFreeCamera() ) {
                    movement.z += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<StopForwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_S: // Start moving backward.
                if ( cam->isFreeCamera() ) {
                    movement.z -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<StopBackwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_A: // Start moving left.
                if ( cam->isFreeCamera() ) {
                    movement.x += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<StopLeftEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_D: // Start moving right.
                if ( cam->isFreeCamera() ) {
                    movement.x -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->pushEvent( std::make_shared<StopRightEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_Q: // Start moving down.
                if ( cam->isFreeCamera() ) {
                    movement.y += CAMERA_MOVEMENT_SPEED;
                }
                break;

            case GLFW_KEY_E: // Start moving up.
                if ( cam->isFreeCamera() ) {
                    movement.y -= CAMERA_MOVEMENT_SPEED;
                }
                break;

        }
    }

}

glm::vec3 Window::trackBallMapping( float x, float y ) {

    glm::vec3 map;
    map.x = ( 2.0f * x - Window::width ) / Window::width; // Find X and Y
    map.y = ( Window::height - 2.0f * y ) / Window::height;
    map.z = 0.0f;

    map.z = std::sqrtf( 1.001f - std::powf( std::min( glm::length( map ), 1.0f ), 2 ) ); // Calculate Z

    map = glm::normalize( map ); // Normalize

    return map;

}

static bool rotating = false;

// For when we want the movement of the mouse itself to move something
void Window::mouse_move_callback( GLFWwindow *, double xpos, double ypos ) {

    static double lastX = 0, lastY = 0;

    if ( rotating ) { // Translate according to mouse movement on virtual trackball
        glm::vec3 last = trackBallMapping( ( float ) lastX, ( float ) lastY );
        glm::vec3 cur = trackBallMapping( ( float ) xpos, ( float ) ypos );
        float velocity = glm::length( cur - last );
        if ( velocity >= ROTATION_THRESHOLD ) { // Ignore trivially small rotations
            glm::vec3 rotationAxis = glm::cross( last, cur ); // Calculate rotation axis in camera frame.
            glm::mat4 toWorld = glm::inverse( cam->getV() ); // Calculate transform from camera to world frame.
            toWorld[3].x = 0.0f; // Remove translation component.
            toWorld[3].y = 0.0f;
            toWorld[3].z = 0.0f;
            rotationAxis = toWorld * glm::vec4( rotationAxis, 1.0f ); // Convert axis to world frame.
        #pragma warning( push )
        #pragma warning( disable: 4189 )
            float angle = velocity * MOUSE_ROTATE_SCALE;
        #pragma warning( pop )
            // TODO: Move something
        }
    }

    lastX = xpos;
    lastY = ypos;

}

void Window::mouse_button_callback( GLFWwindow *, int button, int action, int /* mods */ ) {

    switch ( button ) {

        case GLFW_MOUSE_BUTTON_LEFT:
            if ( action == GLFW_PRESS ) {
                rotating = true;
            } else {
                rotating = false;
            }
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            // Nothing
            break;

    }

}

void Window::mouse_scroll_callback( GLFWwindow *, double /* xoffset */, double /* yoffset */ ) {

    // Nothing

}
