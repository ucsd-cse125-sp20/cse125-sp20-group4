#version 330 core

layout ( location = 0 ) in vec3 position;

out vec2 textureCoords;

void main() {

	// Just pass on position.
    gl_Position = vec4( position, 1.0 );
	textureCoords = ( position.xy + vec2( 1, 1 ) ) / 2.0;

}