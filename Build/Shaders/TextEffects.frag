#version 130

in vec2 vertTextureCoordinate;

uniform sampler2D texTexture0;
uniform vec2 uniTextureDimensions;

uniform int uniUseShadow;
uniform int uniUseGlow;

out vec4 fragColor;

float GetShadowSample(vec2 a_Offset)
{
	vec2 shadow_offset = a_Offset / uniTextureDimensions;
	vec4 shadow_sample = texture2D(texTexture0, vertTextureCoordinate + shadow_offset);
	
	return shadow_sample.a;
}

float GetBlurSample(float a_Blur)
{
	vec2 blur_dimensions = a_Blur / uniTextureDimensions;

	float sample_offsets[9] = float[](
		-4.0,
		-3.0,
		-2.0,
		-1.0,
		0.0,
		1.0,
		2.0,
		3.0,
		4.0
	);
	float sample_weights[9] = float[](
		0.0162162162, 
		0.0540540541, 
		0.1216216216,
		0.1945945946,
		0.2270270270,
		0.1945945946,
		0.1216216216,
		0.0540540541,
		0.0162162162
	);
	
	float sampled = 0.0;
	
	for (int y = 0; y < 9; ++y)
	{
		float sample_vertical = texture2D(texTexture0, vertTextureCoordinate + (vec2(0.0, sample_offsets[y] * blur_dimensions.y))).a * sample_weights[y];
		
		for (int x = 0; x < 9; ++x)
		{
			sampled += sample_vertical + texture2D(texTexture0, vertTextureCoordinate + (vec2(sample_offsets[x] * blur_dimensions.x, 0.0))).a * sample_weights[x];
		}
	}
	
	return sampled;
}

void main()
{
	// because of bugs in NVIDIA's implementation,
	// the colors for shadow and glow must be hard-coded
	// or they won't work properly
	
	vec4 color_text = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 color_glow = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 color_shadow = vec4(0.6, 0.6, 0.6, 1.0);
	
	vec4 sample_text = color_text * texture(texTexture0, vertTextureCoordinate).a;
	vec4 sample_final = sample_text;
	
	if (uniUseGlow == 1)
	{
		float blur = 0.5;
		
		vec4 sample_glow = color_glow * GetBlurSample(blur);
		
		sample_final = mix(sample_glow, sample_final, sample_final.a);
	}
	
	if (uniUseShadow == 1)
	{
		vec2 offset = vec2(-2.0, -1.0);
		vec4 sample_shadow = color_shadow * GetShadowSample(offset);
		
		sample_final = mix(sample_shadow, sample_final, sample_final.a);
	}
	
	fragColor = sample_final;
}