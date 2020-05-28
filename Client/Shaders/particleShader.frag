#version 330 core

in vec4 Color;

out vec4 color;

uniform sampler2D sprite;

void main()
{
    color = texture(sprite, gl_PointCoord) * Color;
}