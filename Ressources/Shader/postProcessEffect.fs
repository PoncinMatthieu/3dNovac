
#version 330 core
precision highp float;

uniform sampler2D	Frame;
uniform bool		SketchActivated;
uniform sampler2D	SketchSampler;
uniform bool		SketchColored;
uniform float		SketchThreshold;
uniform float		SketchBrightness;
uniform vec2		SketchJitter;

in Vertice
{
  vec2	TexCoord;
}	vertice;

out vec4	FragmentColor;

void main()
{
	FragmentColor = texture(Frame, vertice.TexCoord);
	
	if (SketchActivated)
	{
		// Adjust the scene color to remove very dark values and increase the contrast.
		vec4 saturatedScene = clamp((FragmentColor - SketchThreshold) * 2, 0.0, 1.0);

		// Look up into the sketch pattern overlay texture.
		vec4 sketchPattern = texture(SketchSampler, vertice.TexCoord + SketchJitter);

		// Convert into negative color space, and combine the scene color with the
		// sketch pattern. We need to do this multiply in negative space to get good
		// looking results, because pencil sketching works by drawing black ink
		// over an initially white page, rather than adding light to an initially
		// black background as would be more common in computer graphics.
		vec4 negativeSketch = (1 - saturatedScene) * (1 - sketchPattern);

		// Convert the result into a positive color space greyscale value.
		float sketchResult = dot(1 - negativeSketch, vec4(SketchBrightness, SketchBrightness, SketchBrightness, SketchBrightness));
		
		// Apply the sketch result to the main scene color.
		if (SketchColored)
			FragmentColor *= sketchResult;
		else
			FragmentColor = vec4(sketchResult, sketchResult, sketchResult, sketchResult);
	}
}
