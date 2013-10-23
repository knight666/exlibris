#version 330 core

in vec2 vertTextureCoordinate;

uniform sampler2D texTexture0;
uniform vec2 uniTextureDimensions;

out vec4 fragColor;

void main()
{
	vec4 color_text = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 color_shadow = vec4(0.25, 0.25, 0.25, 1.0);
	
	vec4 color_sample = texture(texTexture0, vertTextureCoordinate);
	
	vec2 shadow_offset = vec2(-1.0, -1.0) / uniTextureDimensions;
	vec4 shadow_sample = color_shadow * texture(texTexture0, vertTextureCoordinate + shadow_offset).a;
	
	fragColor = mix(shadow_sample, color_sample, color_sample.a);
}