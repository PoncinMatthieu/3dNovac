#version 330 core

uniform mat4    MVPMatrix;

in vec3         InCoord;
in vec3         InColor;
in vec3         InNormal;

out Vertice
{
    vec3        Color;
    vec3        Normal;
}   vertice;

void main ()
{
    vertice.Color = InColor;
    vertice.Normal = vec3(MVPMatrix * vec4(InNormal, 0.0));
    normalize(vertice.Normal);
    gl_Position = MVPMatrix * vec4(InCoord, 1.0);
}
