// Cli+.cpp : Defines the entry point for the application.
//

#include <iostream>

//#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include "GL/glew.h"
#endif
#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"
#include <winsock2.h>
#include <ws2tcpip.h>


#include "logger.h"
#include "server.h"
#include "Window.h"
#include "ObjectClasses/object.h"
#include "deserializer.h"

#pragma comment (lib, "Ws2_32.lib")

#define LOGFILE_NAME "log/client.log"
#define LOGLEVEL spdlog::level::trace //was debug
#define DEFAULT_PORT "8080"
#define DEFAULT_BUFLEN 512

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
    WSADATA wsaData;
    SOCKET ServerSocket = INVALID_SOCKET;
    Server* server = NULL;
    char outbuf[DEFAULT_BUFLEN] = { 0 };
    char inbuf[DEFAULT_BUFLEN] = { 0 };
    concurrency::concurrent_queue<std::shared_ptr<Event>> eventQueue = concurrency::concurrent_queue<std::shared_ptr<Event>>(); //todo use shared data class insteads of int
    int status;

    //initialize Winsock
    if ((status = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        spdlog::critical("WSAStartup failed with error: {0:d}", status);
        return EXIT_FAILURE;
    }
    struct addrinfo* result = NULL;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // resolve server address and port (current server address is local host)
    if ((status = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result)) != 0) {
        spdlog::critical("getaddrinfo failed with error: {0:d}", status);
        WSACleanup();
        return EXIT_FAILURE;
    }

    ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ServerSocket == INVALID_SOCKET) {
        spdlog::critical("socket failed with error: {0:ld}", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return EXIT_FAILURE;
    }

    if ((status = connect(ServerSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) {
        closesocket(ServerSocket);
        ServerSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ServerSocket == INVALID_SOCKET) {
        spdlog::critical("unable to connect to server!");
        WSACleanup();
        return EXIT_FAILURE;
    }
    spdlog::info("Connected to server");

    u_long mode = 1; //enable non blocking
    ioctlsocket(ServerSocket, FIONBIO, &mode);
    server = new Server(ServerSocket, &eventQueue);
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
    Window::initialize(server);
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Object>>> updates = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Object>>>();
    // Loop while GLFW window should stay open
    std::string leftover = "";
    while ( !glfwWindowShouldClose( window ) ) {
        Deserializer deserializer;
       
        int bytes = server->recv( inbuf, DEFAULT_BUFLEN );
        if ( bytes == SOCKET_ERROR ) {
            spdlog::info( "Socket error: {}", WSAGetLastError() );
        } else {
            std::string in_str( inbuf, bytes );
            memset( inbuf, 0, sizeof( inbuf ) );
            spdlog::trace( "Received message from server: {0}", in_str ); //only for testing, can be removed

            // todo push game state onto event queue
            std::string message = leftover + in_str;
            spdlog::trace( "Full message: {0}", message ); //only for testing, can be removed
            leftover = deserializer.deserializeUpdates( message, updates );
            spdlog::trace( "Message leftover: {}", leftover );
            if ( !updates->empty() ) {
                //@Thiago this pointer to unordered map gives you ID to object mappings, and objects give you position/direction TODO
                spdlog::debug( "Number of updates: {}", updates->size() );
                Window::world->handleUpdates( updates );
                updates->clear();
            }
        }

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