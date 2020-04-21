#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 model; // Model matrix, instanced.

const int NUM_CASCADES = 3;

out vec3 Normal;
out vec3 FragPos;
out vec4 ShadowCoord[NUM_CASCADES];
out float shadowClipZ;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 shadow[NUM_CASCADES];

void main() {

	vec4 viewPos = view * model * vec4( position, 1.0f );
    gl_Position = projection * viewPos;
    FragPos = vec3( model * vec4( position, 1.0f ) );

    Normal = vec3( transpose( inverse( model ) ) * vec4( normal, 1.0f ) );

	for ( int i = 0; i < NUM_CASCADES; i++ ) {

		ShadowCoord[i] = shadow[i] * model * vec4( position, 1.0f );

	}
	shadowClipZ = -viewPos.z;

} 
