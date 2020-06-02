#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 model; // Model matrix, instanced.

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 view;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec4 vertexColor;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
	vec3 color = normalize( normal );
	color.x = color.x * 0.5f + 0.5f;
	color.y = color.y * 0.5f + 0.5f;
	color.z = color.z * 0.5f + 0.5f;
    vertexColor = vec4( color.x, color.y, color.z, 1.0 );
}
