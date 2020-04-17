#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 model; // Model matrix, instanced.

// Uniform variables.
uniform mat4 projection;
uniform mat4 view;

// Output color of fragment.
out vec4 vertexColor;

void main() {

    vec4 pos = gl_Position = projection * mat4( mat3( view ) ) * model * vec4( position, 1.0 );
	gl_Position = pos.xyww; // Ensure position is as far back as possible.
    vertexColor = vec4( color, 1.0f ); // Pass-through color.

}
