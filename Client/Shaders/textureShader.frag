#version 330 core

out vec4 vertexColor;

uniform sampler2D textureID;

in vec2 textureCoords;

void main() {

	float depthValue = texture( textureID, textureCoords ).r;
    vertexColor = vec4( vec3( depthValue ), 1.0 );

}