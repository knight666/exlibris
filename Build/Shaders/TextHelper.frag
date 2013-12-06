#version 330 core

in vec2 vertTextureCoordinate;

uniform sampler2D texTexture0;
uniform vec2 uniTextureDimensions;
uniform vec4 uniTextColor;

out vec4 fragColor;

void main()
{
	vec4 color_sample = uniTextColor * texture(texTexture0, vertTextureCoordinate).a;
	
	vec2 shadow_offset = vec2(-1.0, -1.0) / uniTextureDimensions;
	vec4 shadow_sample = vec4(0.25, 0.25, 0.25, texture(texTexture0, vertTextureCoordinate + shadow_offset).a);
	
	fragColor = mix(shadow_sample, color_sample, color_sample.a);
}