#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
//layout (location = 2) in vec3 normal;
//layout (location = 3) in mat4 model; // Model matrix, instanced.

// Uniform variables.
uniform mat4 projection;
uniform mat4 view;

// Output color of fragment.
out vec4 vertexColor;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    //gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
    //vertexColor = vec4( color, 1.0f ); // Pass-through color.

    gl_Position = projection * view * vec4(position.x, position.y, position.z, 1.0);
    vertexColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f );
}
