// Cli+.cpp : Defines the entry point for the application.
//

#include <iostream>

//#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <combaseapi.h>

#include <fmod_studio.hpp>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>


#include "logger.h"
#include "Server.h"
#include "Window.h"
#include "ObjectClasses/object.h"
#include "deserializer.h"

#pragma comment (lib, "Ws2_32.lib")

#define LOGFILE_NAME "log/client.log"
#define LOGLEVEL spdlog::level::trace //was debug
#define DEFAULT_PORT "8080"
#define DEFAULT_BUFLEN 512

// FMOD constants
#define AUDIO_CHANNELS_MAX 100

void setup_glew() {

    // Initialize GLEW. Not needed on OSX systems.
#ifndef __APPLE__
    GLenum err = glewInit();
    if ( GLEW_OK != err ) {
        /* Problem: glewInit failed, something is seriously wrong. */
        spdlog::critical( "Error: {}", glewGetErrorString( err ) );
        glfwTerminate();
    }
    spdlog::info( "Current GLEW version: {}", glewGetString( GLEW_VERSION ) );
#endif

}

void setup_opengl_settings() {

#ifndef __APPLE__
    // Setup GLEW. Don't do this on OSX systems.
    setup_glew();
#endif
    // Enable depth buffering
    glEnable( GL_DEPTH_TEST );
    // Related to shaders and z value comparisons for the depth buffer
    glDepthFunc( GL_LEQUAL );
    // Set polygon drawing mode to fill front and back of each polygon
    // You can also use the paramter of GL_LINE instead of GL_FILL to see wireframes
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    // Enable backface culling to render only one side of polygons
    glEnable( GL_CULL_FACE );
    // Do not render back side
    glCullFace( GL_BACK );
    // Set clear color
    glClearColor( 0.05f, 0.8f, 0.85f, 1.0f );

#ifdef DEBUG_MODE
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( ( GLDEBUGPROC ) openGLErrorCallback, 0 );
#ifndef TRACE_MODE
    glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, false );
#endif
#endif

}

void print_versions() {
    // Get info of GPU and supported OpenGL version
    spdlog::info( "Renderer: {}", glGetString( GL_RENDERER ) );
    spdlog::info( "OpenGL version supported {}", glGetString( GL_VERSION ) );

    //If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
    spdlog::info( "Supported GLSL version is {}.", ( char * ) glGetString( GL_SHADING_LANGUAGE_VERSION ) );
#endif
}

void error_callback( int error, const char * description ) {

    // Print error
    spdlog::error( "Error code {}: {}", error, description );

}

void setup_callbacks( GLFWwindow * window ) {

    // Set the error callback
    glfwSetErrorCallback( error_callback );
    // Set the key callback
    glfwSetKeyCallback( window, Window::key_callback );
    // Set the mouse callbacks
    glfwSetCursorPosCallback( window, Window::mouse_move_callback );
    glfwSetMouseButtonCallback( window, Window::mouse_button_callback );
    glfwSetScrollCallback( window, Window::mouse_scroll_callback );
    // Set the window resize callback
    glfwSetWindowSizeCallback( window, Window::resize_callback );

}

int main_inner( void ) {

    spdlog::info( "Initializing COM." );
    HRESULT hr = CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
    if ( FAILED( hr ) ) {
        spdlog::critical( "Failed to initialize COM, error code {}.", hr );
        return 1;
    }

    spdlog::info( "Initializing FMOD." );
    FMOD::Studio::System * audioSystem;
    FMOD_RESULT res = FMOD::Studio::System::create( &audioSystem );
    if ( res != FMOD_RESULT::FMOD_OK ) {
        spdlog::critical( "Could not initialize FMOD ({}).", res );
    }
    audioSystem->initialize( AUDIO_CHANNELS_MAX, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr );

    //initialize Winsock
    WSADATA wsaData;
    int status;
    if ( ( status = WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) ) != 0 ) {
        spdlog::critical( "WSAStartup failed with error: {0:d}", status );
        return EXIT_FAILURE;
    }

    Server * server = new Server( "127.0.0.1", DEFAULT_PORT );

    // Create the GLFW window
    spdlog::info( "Creating window..." );
    GLFWwindow * window = Window::create_window( 640, 480 );

    if ( window == NULL ) {
        spdlog::critical( "Failed to create window!" );
        return EXIT_FAILURE;
    } else {
        spdlog::info( "Window created." );
    }

    // Print OpenGL and GLSL versions
    print_versions();
    // Setup callbacks
    setup_callbacks( window );
    // Setup OpenGL settings, including lighting, materials, etc.
    setup_opengl_settings();
    // Initialize objects/pointers for rendering
    Window::initialize( server );
    // Loop while GLFW window should stay open
    while ( !glfwWindowShouldClose( window ) ) {

        // Main render display callback. Rendering of objects is done here.
        Window::display_callback( window );
        // Idle callback. Updating objects, etc. can be done here.
        Window::idle_callback();

    }

    Window::clean_up();
    // Destroy the window
    glfwDestroyWindow( window );
    // Terminate GLFW
    glfwTerminate();

    delete( server );
    WSACleanup();

    spdlog::info( "Uninitializing FMOD." );
    audioSystem->release();

    spdlog::info( "Uninitializing COM." );
    CoUninitialize();

    return EXIT_SUCCESS;

}

int main( void ) {

    initLogging( LOGFILE_NAME, LOGLEVEL );
    spdlog::info( "Client starting up." );
    try {
        int statusCode = main_inner();
        spdlog::info( "Client shutting down." );
        shutdownLogging();
        return statusCode;
    } catch ( std::exception & e ) {
        spdlog::critical( "Unhandled exception: {}", e.what() );
        shutdownLogging();
        throw e; // Record and rethrow
    }

}