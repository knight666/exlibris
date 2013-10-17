#version 130

in vec2 vertTextureCoordinate;

uniform sampler2D texTexture0;
uniform vec2 uniTextureDimensions;

out vec4 fragColor;

void main()
{
	float blur = 0.5;
	vec2 blur_dimensions = blur / uniTextureDimensions;

	float sample_offsets[9] = float[](-4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0);
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
	
	vec4 texel = vec4(1.0, 1.0, 1.0, texture2D(texTexture0, vertTextureCoordinate).a);
	
	vec4 glow_color = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 texel_glow = glow_color * sampled;

	fragColor = mix(texel_glow, texel, texel.a);
}