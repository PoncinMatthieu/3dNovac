#version 330 core

in Vertice
{
    vec3        Color;
    vec3        Normal;
}   vertice[];

out Normal
{
    vec3	Color;
}   normal;

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
       gl_Position = gl_in[i].gl_Position;
       normal.Color = vertice[i].Color;
       EmitVertex();
       gl_Position = gl_in[i].gl_Position + vec4(vertice[i].Normal, 0.f);
       normal.Color = vertice[i].Color;
       EmitVertex();
       EndPrimitive();
    }
}
