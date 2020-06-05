#version 330 core

in vec4 Color;

out vec4 color;

uniform sampler2D sprite;

void main()
{
    if(Color.a < 0.1){
        discard;
    }
    color = texture(sprite, gl_PointCoord) * Color;
}