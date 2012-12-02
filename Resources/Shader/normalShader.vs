#version 330 core

uniform mat4    MVPMatrix;

in vec3         InCoord;
in vec4         InColor;
in vec3         InNormal;

out Vertice
{
    vec4        Color;
    vec3        Normal;
}   vertice;

void main ()
{
    vertice.Color = InColor;
    vertice.Normal = InNormal;
    gl_Position = vec4(InCoord, 1.0);
}
