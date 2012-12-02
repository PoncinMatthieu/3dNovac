#version 330 core
precision highp float;

in Normal
{
    vec4        Color;
}   vertice;

out vec4            FragmentColor;

void main()
{
    FragmentColor = vertice.Color;
}
