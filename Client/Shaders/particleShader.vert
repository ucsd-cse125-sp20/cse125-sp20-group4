#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 pv;
uniform vec2 offset;
uniform vec4 color;

void main()
{
    float scale = 2.0f;
    TexCoords = vertex.wz;
    ParticleColor = color;
    gl_Position = pv * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}