#version 330 core

// Receive color from vertex shaders.
in vec4 vertexColor;
in vec2 coord;

// Output color of fragment.
out vec4 fragColor;

uniform sampler2D sprite;

void main() {

    // Use color interpolated from vertex colors.
    fragColor = vertexColor * texture(sprite, coord);

}
