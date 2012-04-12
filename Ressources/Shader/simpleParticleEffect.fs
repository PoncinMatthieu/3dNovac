#version 330 core
precision highp float;

in Vertice
{
  vec4		Color;
} vertice;

out vec4        OutColor;

void main()
{
  OutColor = vertice.Color;
}
