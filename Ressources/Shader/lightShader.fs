#version 330 core
precision highp float;

uniform bool        LightPass;
uniform bool        Textured;
uniform bool	    BumpMapping;
uniform sampler2D   Diffuse;
uniform sampler3D   LightMap;
uniform sampler2D   NormalMap;

in Vertice
{
    vec2        TexCoord;
    vec4        Color;
    vec3        Normal;
    vec3        ToLight;
    vec3        Attenuation;
}   vertice;

out vec4            FragmentColor;

void main()
{
    FragmentColor = vertice.Color;

    if (LightPass)
    {
	// normalize vectors
        normalize(vertice.Normal);
        normalize(vertice.ToLight);

	float NdotL;
	if (BumpMapping)  // NdotL with the normal map
	{
            // retreive the NormalTexture in the normal map
	    vec3 Nt = texture(NormalMap, vertice.TexCoord).rgb;
            Nt = (Nt * 2) - 1; // remap the normal texture in [0..1] insteed of [0.5..1]

	    // compute the NormalFinal with a vecZ of reference
            vec3 Nf = Nt + vertice.Normal;
	    Nf.z -= 1;

            NdotL = dot(Nf, vertice.ToLight);
	}
	else  // simple NdotL
            NdotL = dot(vertice.Normal, vertice.ToLight);
        FragmentColor *= max(NdotL, 0.0);

	// attenuation with the light map
        FragmentColor *= texture(LightMap, vertice.Attenuation);
    }
    if (Textured)
        FragmentColor *= texture(Diffuse, vertice.TexCoord);
}
