#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

// Uniform variables.
uniform mat4 model;
uniform mat4 pv;

// Output color of fragment.
out vec4 vertexColor;

void main() {

    gl_Position = pv * model * vec4( position.x, position.y, position.z, 1.0 );
    vertexColor = vec4( color, 1.0f );

}
