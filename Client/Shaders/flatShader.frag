#version 330 core

// Receive color from vertex shaders.
in vec4 vertexColor;

// Output color of fragment.
out vec4 fragColor;

void main()
{
    // Use color interpolated from vertex colors.
    fragColor = vertexColor;
}
