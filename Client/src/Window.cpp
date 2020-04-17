#include <algorithm>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Shaders.h"

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

const char * window_title = "CSE 125 Project";

#define RADIANS( W ) ( W ) * ( glm::pi<float>() / 180.0f )
#define PRINT_VECTOR( V ) V.x << "|" << V.y << "|" << V.z

#define DEFAULT_CAMERA_POS glm::vec3( 0.0f, 0.0f, -5.0f )
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

// Default camera parameters
glm::vec3 Window::cam_pos = DEFAULT_CAMERA_POS; // Position of camera.
glm::vec3 Window::cam_dir = DEFAULT_CAMERA_DIR; // Direction of the camera.

#define ROTATE( direction, angle, axis ) ( glm::rotate( glm::mat4( 1.0f ), ( angle ), ( axis ) ) * glm::vec4( ( direction ), 1.0f ) )

void Window::rotateCamera( float angle, glm::vec3 axis ) {

    cam_dir = ROTATE( cam_dir, angle, axis );
    setCamera();

}

void Window::setCamera() {

    glm::vec3 cam_look_at = cam_pos + cam_dir;
    glm::vec3 cam_up = glm::cross( glm::cross( cam_dir, glm::vec3( 0.0f, 1.0f, 0.0f ) ), cam_dir );
    V = glm::lookAt( cam_pos, cam_look_at, cam_up );

}

GLFWwindow * Window::window = nullptr;

int Window::width;
int Window::height;

float Window::nearZ = 0.1f;
float Window::farZ = 300.0f;
float Window::fov = RADIANS( 45.0f );
float Window::aspect = 0.0f;

glm::mat4 Window::P;
glm::mat4 Window::V;

static GLuint VAO, EBO, VBO;

void Window::initialize() {

    Shaders::initializeShaders();

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

}

void Window::clean_up() {

    Shaders::deleteShaders();

    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &EBO );
    glDeleteBuffers( 1, &VBO );

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
        aspect = ( float ) newWidth / ( float ) newHeight;
        P = glm::perspective( fov, aspect, nearZ, farZ );
        setCamera();
    }

}

static glm::vec3 movement( 0.0f, 0.0f, 0.0f ); // Camera movement direction.

void Window::idle_callback() {

    // Translate camera
    if ( ( movement.x != 0.0f ) || ( movement.y != 0.0f ) || ( movement.z != 0.0f ) ) {
        glm::vec4 move = glm::inverse( V ) * glm::vec4( movement, 1.0f );
        cam_pos = glm::vec3( move ); // Camera is moving.
        setCamera();
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
        rotateCamera( Y_SPEED( ( float ) cursorY ), glm::cross( glm::vec3( 0.0f, 1.0f, 0.0f ), cam_dir ) );
    }
#pragma warning( pop )

}

void Window::display_callback( GLFWwindow * ) {

    // Clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Dunno if actually needed

    Shader shaderProgram = Shaders::flatShader();
    glUseProgram( shaderProgram );

    // Render scene.
    // TODO: Real strategy

    // Send projection and view matrices to shader programs.
    glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "projection" ), 1, GL_FALSE, &Window::P[0][0] );
    glUniformMatrix4fv( glGetUniformLocation( shaderProgram, "view" ), 1, GL_FALSE, &Window::V[0][0] );
    glUniform3fv( glGetUniformLocation( shaderProgram, "viewPos" ), 1, &Window::cam_pos.x );

    // Bind vertex array.
    glBindVertexArray( VAO );

    std::vector<float> data;

    for ( int i = -1; i <= 1; i += 2 ) {
        for ( int j = -1; j <= 1; j += 2 ) {
            for ( int k = -1; k <= 1; k += 2 ) {
                data.push_back( i * 1.0f );
                data.push_back( j * 1.0f );
                data.push_back( k * 1.0f );
            }
        }
    }

    // Now bind a VBO to it as a GL_ARRAY_BUFFER (drawing data).
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    // Populate buffer with vertex data.
    glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof( float ), data.data(), GL_STREAM_DRAW );

    // Position.
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, // Attribute ID.
        3, // How many components there are per vertex.
        GL_FLOAT, // What type these components are.
        GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't.
        3 * sizeof( GLfloat ), // Offset between consecutive indices.
        ( GLvoid * ) 0 ); // Offset of the first vertex's component.

    // Bind GL_ELEMENT_ARRAY_BUFFER for drawing order.
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );

    std::vector<unsigned int> indices;

    // Left face
    indices.push_back( 0 );
    indices.push_back( 2 );
    indices.push_back( 1 );

    indices.push_back( 1 );
    indices.push_back( 2 );
    indices.push_back( 3 );

    // Right face
    indices.push_back( 5 );
    indices.push_back( 7 );
    indices.push_back( 4 );

    indices.push_back( 4 );
    indices.push_back( 7 );
    indices.push_back( 6 );

    // Back face
    indices.push_back( 0 );
    indices.push_back( 4 );
    indices.push_back( 2 );

    indices.push_back( 2 );
    indices.push_back( 4 );
    indices.push_back( 6 );

    // Front face
    indices.push_back( 1 );
    indices.push_back( 3 );
    indices.push_back( 5 );

    indices.push_back( 3 );
    indices.push_back( 7 );
    indices.push_back( 5 );

    // Bottom face
    indices.push_back( 0 );
    indices.push_back( 1 );
    indices.push_back( 5 );

    indices.push_back( 0 );
    indices.push_back( 5 );
    indices.push_back( 4 );

    // Top Face
    indices.push_back( 2 );
    indices.push_back( 6 );
    indices.push_back( 3 );

    indices.push_back( 3 );
    indices.push_back( 6 );
    indices.push_back( 7 );

    // Buffer drawing order.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), indices.data(), GL_STREAM_DRAW );

    // Tell OpenGL to draw with triangles, using the recorded amount of indices and no offset.
    glDrawElements( GL_TRIANGLES, ( GLsizei ) indices.size(), GL_UNSIGNED_INT, 0 );

    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers.
    glBindVertexArray( 0 );

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
                movement.z -= CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_S: // Start moving backward.
                movement.z += CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_A: // Start moving left.
                movement.x -= CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_D: // Start moving right.
                movement.x += CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_Q: // Start moving down.
                movement.y -= CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_E: // Start moving up.
                movement.y += CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_R: // Reset camera position.
                cam_pos = DEFAULT_CAMERA_POS; // Reset to default position.
                cam_dir = DEFAULT_CAMERA_DIR; // Set direction too.
                setCamera();
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
                    std::cout << "Current camera position: " << PRINT_VECTOR( cam_pos ) << std::endl;
                    std::cout << "Current camera direction: " << PRINT_VECTOR( cam_dir ) << std::endl;
                }
                break;

        }
    } else if ( action == GLFW_RELEASE ) { // Check for a key release.
        switch ( key ) {

            case GLFW_KEY_W: // Stop moving forward.
                movement.z += CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_S: // Stop moving backward.
                movement.z -= CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_A: // Stop moving left.
                movement.x += CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_D: // Stop moving right.
                movement.x -= CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_Q: // Stop moving down.
                movement.y += CAMERA_MOVEMENT_SPEED;
                break;

            case GLFW_KEY_E: // Stop moving up.
                movement.y -= CAMERA_MOVEMENT_SPEED;
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
            glm::mat4 toWorld = glm::inverse( V ); // Calculate transform from camera to world frame.
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
