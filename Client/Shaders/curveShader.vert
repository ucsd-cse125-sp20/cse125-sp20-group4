#version 330 core

layout (location = 0) in vec3 position;

// Uniform variables.
uniform mat4 projection;
uniform mat4 view;

void main() {

    // Calculate projected position.
    gl_Position = projection * view * vec4(position.x, position.y, position.z, 1.0);

}
