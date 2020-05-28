#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;

out vec4 Color;

uniform mat4 model;
uniform mat4 pv;

uniform float scale;

uniform vec3 cameraPos;

const float minPointScale = 0.1;
const float maxPointScale = 0.9;
const float maxDistance   = 10.0;

void main()
{
    vec4 pos = pv * vec4(vertex, 1.0) * scale;
    gl_Position = pos;

    float cameraDist = distance(vertex, cameraPos);
    float pointScale = 1.0 - (cameraDist / maxDistance);
    pointScale = max(pointScale, minPointScale);
    pointScale = min(pointScale, maxPointScale);
    gl_PointSize = pointScale * (100 + (1 - color.a) * 10);

    Color = color;
}