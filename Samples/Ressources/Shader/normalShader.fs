#version 330 core
precision highp float;

in Normal
{
    vec3        Color;
}   vertice;

out vec4            FragmentColor;

void main()
{
    FragmentColor = vec4(vertice.Color, 1.0);
}
