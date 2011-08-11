#version 330 core

uniform vec4    LightColor;
uniform bool    LightPass;
uniform mat4    VPMatrix;
uniform mat4    MMatrix;
uniform mat4	NormalMatrix;
uniform vec4    Light; // coord light  + rayon dans w

in vec3         InCoord;
in vec2         InTexCoord;
in vec3         InColor;
in vec3         InNormal;

out Vertice
{
    vec2        TexCoord;
    vec4        Color;
    vec3        Normal;
    vec3        ToLight;
    vec3        Attenuation;
}   vertice;

void main ()
{
    vec4 vPos = MMatrix * vec4(InCoord, 1.0);
    if (!LightPass)
        vertice.Color = vec4(InColor, 1.0) * LightColor;
    else
    {
        vertice.Color = vec4(InColor, 1.0) * LightColor;
        vertice.Normal = vec3(NormalMatrix * vec4(InNormal, 1.0));
        vertice.ToLight = Light.xyz - vPos.xyz;
        normalize(vertice.Normal);
	normalize(vertice.ToLight);

        // on calcule l'attenuation
        // on divise le vecteur par le rayon de la lumiere (w contient l'inverse du rayon)
        // et on ajoute 0.5 pour partir du centre de la texture
        vertice.Attenuation = ((vPos.xyz - Light.xyz) * Light.w) + 0.5;
    }

    vertice.TexCoord = InTexCoord;
    gl_Position = VPMatrix * vPos;
}
