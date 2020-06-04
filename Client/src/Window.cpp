#define NOMINMAX // Stop stupid library from defining max() as a macro

#pragma warning(disable:4201)

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include <EventClasses/events.h>
#include <logger.h>

#include "Window.h"
#include "MapLoader.h"
#include "drawing/Shaders.h"
#include "drawing/model/Axis.h"
#include "drawing/model/EmptyModel.h"
#include "drawing/model/RectangularCuboid.h"
#include "drawing/model/LoadedModel.h"
#include "state/CameraEntity.h"
#include "state/Entity.h"

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

static const auto LOGGER = getLogger( "Window" );

static const char * window_title = "CSE 125 Project";

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
ParticleManager* Window::pmanager;

// Audio data
FMOD::Studio::System * Window::audioSystem;
FMOD::Studio::Bank * Window::bankMaster;
FMOD::Studio::Bank * Window::bankMasterStrings;

std::string Window::playerName = "cube4"; 

GLFWwindow* Window::window = nullptr;

int Window::width;
int Window::height;
int Window::money;
int Window::holding;
bool Window::ready;
Entity* Window::selected;

FMOD::Studio::EventDescription * Window::ambientMusic;
FMOD::Studio::EventInstance * Window::ambientMusicEvent;

void Window::rotateCamera( float angle, glm::vec3 axis ) {

    bool isPlayer = cam->name == Window::playerName;
    if ( cam->isFreeCamera() || isPlayer ) {
        glm::vec3 newDir = ROTATE( cam->getDir(), angle, axis );
        cam->rotate( newDir );
        if ( isPlayer ) { // Send player movement to server
            server->send( std::make_shared<RotateEvent>( playerName, newDir ) );
        }
    }

}

void Window::set3DParams( FMOD_3D_ATTRIBUTES & attr, const glm::vec3 & position, const glm::vec3 & velocity, const glm::vec3 & direction ) {

    // Position
    attr.position.x = position.x;
    attr.position.y = position.y;
    attr.position.z = position.z;

    // Velocity
    attr.velocity.x = velocity.x;
    attr.velocity.y = velocity.y;
    attr.velocity.z = velocity.z;

    // Direction
    attr.forward.x = direction.x;
    attr.forward.y = direction.y;
    attr.forward.z = direction.z;

    // Up
    static const glm::vec3 UP( 0.0f, 1.0f, 0.0f );
    glm::vec3 up = glm::normalize( glm::cross( glm::cross( direction, UP ), direction ) );
    attr.up.x = up.x;
    attr.up.y = up.y;
    attr.up.z = up.z;

}


void Window::initialize( Server * ser, FMOD::Studio::System * audio ) {
    
    Window::money = 100;
    Window::holding = 0;
    Window::ready = false;
    // Set up graphics
    Shaders::initializeShaders();

    // Set up sound
    Window::audioSystem = audio;

    // Load audio banks
    LOGGER->info( "Loading audio banks." );
    
    FMOD_RESULT res = audioSystem->loadBankFile( "Sounds/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &bankMaster );
    if ( res != FMOD_OK ) {
        LOGGER->critical( "Could not load master bank ({}).", res );
        throw std::runtime_error( "Failed to initialize audio." );
    } else {
        LOGGER->info( "Loaded master bank." );
    }

    res = audioSystem->loadBankFile( "Sounds/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &bankMasterStrings );
    if ( res != FMOD_OK ) {
        LOGGER->critical( "Could not load master bank strings ({}).", res );
        throw std::runtime_error( "Failed to initialize audio." );
    } else {
        LOGGER->info( "Loaded master bank strings." );
    }

    // Load audio samples
    bankMaster->loadSampleData();

    // Start background music
    audioSystem->getEvent( "event:/background_music", &ambientMusic );
    ambientMusic->createInstance( &ambientMusicEvent );
    ambientMusicEvent->start();

    // Set up game state
    world = new World();
    server = ser;
    cam = Camera::addCamera( SPECTATOR_CAMERA, DEFAULT_CAMERA_POS, DEFAULT_CAMERA_DIR ); // Static fallback camera
    
    world->addEntity(new Entity("floor", new RectangularCuboid(glm::vec3(0.5f, 0.5f, 0.5f), 1000.0f, 1.0f, 1000.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    auto model = new LoadedModel("Models/barrier.dae", Shaders::phong());
    model->setColor(glm::vec3(0.6f, 0.3f, 0.0f));
    selected = new Entity("selected", model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),0.4f);
    
    pmanager = new ParticleManager();

    // Debugging entities
    //world->addEntity( new Entity( "worldAxis", new Axis(), glm::vec3( 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), 1.0f, true ) );

}

void Window::clean_up() {

    // Clean up game state
    Camera::removeCamera( "spectator" );

    delete( world );

    // Stop background music
    ambientMusicEvent->stop( FMOD_STUDIO_STOP_IMMEDIATE );
    ambientMusicEvent->release();
    ambientMusicEvent = nullptr;

    // Clean up audio
    LOGGER->info( "Unloading audio banks." );
    bankMaster->unload();

    // Clean up graphics
    Shaders::deleteShaders();

}

GLFWwindow * Window::create_window( int windowWidth, int windowHeight ) {

    // Initialize GLFW.
    if ( !glfwInit() ) {
        LOGGER->critical( "Failed to initialize GLFW" );
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
        LOGGER->critical( "Failed to open GLFW window." );
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

    // Handle incoming events
    std::deque<std::shared_ptr<Event>> events;
    server->receiveAll( events );
    LOGGER->debug( "Number of events: {}", events.size() );
    while ( !events.empty() ) {
        
        handleEvent( events.front() );
        events.pop_front();

    }

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

    // Update audio positioning
    FMOD_3D_ATTRIBUTES attributes;
    set3DParams( attributes, cam->getPos(), glm::vec3( 0.0f ), cam->getDir() );
    FMOD_RESULT res = audioSystem->setListenerAttributes( 0, &attributes );
    if ( res != FMOD_OK ) {
        LOGGER->warn( "Error while updating listener position ({}).", res );
    }

    //update particles
    pmanager->update();

}


void Window::display_callback( GLFWwindow * ) {

    // Clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Dunno if actually needed
    if (holding == 4) {
        selected->setPosition(round(lookingAt()) + glm::vec3(0.0f, 0.1f, 0.0f));
        selected->draw(cam->getToView());
    }

    // Render scene.
    world->draw( cam->getToView() );

    UiHandler::drawGui();

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
                    server->send( std::make_shared<MoveForwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_S: // Start moving backward.
                if ( cam->isFreeCamera() ) {
                    movement.z += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->send( std::make_shared<MoveBackwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_A: // Start moving left.
                if ( cam->isFreeCamera() ) {
                    movement.x -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->send( std::make_shared<MoveLeftEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_D: // Start moving right.
                if ( cam->isFreeCamera() ) {
                    movement.x += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->send( std::make_shared<MoveRightEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_SPACE: // Place an object
                if (cam->name == Window::playerName) {
                    if (Window::holding ==  4) {
                        glm::vec3 pos = lookingAt();
                        server->send(std::make_shared<PlaceEvent>(playerName,pos));
                    } else if (holding == 0){
                        // TODO update to select correct item
                        server->send(std::make_shared<PickUpEvent>(playerName, "0"));
                    }
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
                } else if (cam->name == Window::playerName && Window::holding > 0 && Window::holding < 4) {
                    server->send(std::make_shared<UseEvent>(playerName));
                }
                break;

            case GLFW_KEY_R: // Reset camera position.
                if ( cam->isFreeCamera() ) {
                    cam->update( DEFAULT_CAMERA_POS, DEFAULT_CAMERA_DIR );
                } else if (cam->name == Window::playerName) {
                    server->send(std::make_shared<ReadyEvent>(playerName));
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
                    server->send( std::make_shared<StopForwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_S: // Start moving backward.
                if ( cam->isFreeCamera() ) {
                    movement.z -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->send( std::make_shared<StopBackwardEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_A: // Start moving left.
                if ( cam->isFreeCamera() ) {
                    movement.x += CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->send( std::make_shared<StopLeftEvent>( playerName ) );
                }
                break;

            case GLFW_KEY_D: // Start moving right.
                if ( cam->isFreeCamera() ) {
                    movement.x -= CAMERA_MOVEMENT_SPEED;
                } else if ( cam->name == Window::playerName ) {
                    server->send( std::make_shared<StopRightEvent>( playerName ) );
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
glm::vec3 Window::lookingAt() {
    glm::vec3 planePoint(0.0f, 0.0f, 0.0f);
    glm::vec3 planeNormal(0.0f, 1.0f, 0.0f);
    glm::vec3 diff = Window::cam->getPos() - planePoint;
    float prod1 = glm::dot(diff, planeNormal);
    float prod2 = glm::dot(Window::cam->getDir(), planeNormal);
    float prod3 = prod1 / prod2;
    return Window::cam->getPos() - Window::cam->getDir() * prod3;
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

void Window::handleEvent( const std::shared_ptr<Event> & e ) {

    LOGGER->trace( "{} XXXX {}", Window::playerName, e->serialize() );
    if (e->getType() == Event::EventType::JEvent) {
        Window::playerName = e->getObjectId();
        LOGGER->info("Set my ID to {}", e->getObjectId());
    } else if (e->getType() == Event::EventType::GEvent) {
        auto uEvent = std::static_pointer_cast<UpdateEvent>(e);
        world->handleUpdates(uEvent, Window::playerName);
        // update my data
        auto it = uEvent->updates.find(Window::playerName);
        if (it != uEvent->updates.end()) {
            auto player = std::static_pointer_cast<Player>(it->second);
            money = player->getMoney();
            Window::ready = player->ready;
            if (player->getHeldItem() != nullptr) {
                if (player->getHeldItem()->getTag().compare("Red") == 0) {
                    holding = 1;
                } else if (player->getHeldItem()->getTag().compare("Green") == 0) {
                    holding = 2;
                } else if (player->getHeldItem()->getTag().compare("Blue") == 0) {
                    holding = 3;
                } else if (player->getHeldItem()->getTag().compare("Barricade") == 0) {
                    holding = 4;
                }
            } else {
                holding = 0;
            }
            cam = Camera::getCamera(playerName);
            LOGGER->trace("Updated player ");
        } else {
            LOGGER->trace("Failed to find {}", playerName);
        }
    } else if (e->getType() == Event::EventType::PEvent) {
        Phase& p = (std::dynamic_pointer_cast<UpdatePhaseEvent>(e)->phase);
        world->phase.update(p);
    } else {
        world->handleUpdates( e, Window::playerName );
    }
}
