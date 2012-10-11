#version 330 core

uniform mat4    MVPMatrix;

in vec3 InCoord;
in vec2 InTexCoord;

out Vertice
{
  vec2	TexCoord;
}  vertice;

void main()
{
  vertice.TexCoord = InTexCoord;
  gl_Position = MVPMatrix * vec4(InCoord, 1.0);
}
