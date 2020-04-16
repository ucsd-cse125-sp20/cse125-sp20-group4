#version 330 core

// Color of the fragment
out vec4 fragColor;

uniform vec3 color;

void main() {

    // Use fixed color.
    fragColor = vec4( color, 1.0f );

}
