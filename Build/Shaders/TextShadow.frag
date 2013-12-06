#version 330 core

in vec2 vertTextureCoordinate;

uniform sampler2D texTexture0;
uniform vec2 uniTextureDimensions;

out vec4 fragColor;

void main()
{
	vec2 shadow_offset = vec2(-2.0, -1.0) / uniTextureDimensions;
	vec4 shadow_frag = texture2D(texTexture0, vertTextureCoordinate + shadow_offset);
	vec4 color_frag = texture2D(texTexture0, vertTextureCoordinate);
	
	vec4 text_color = vec4(1.0, 1.0, 1.0, color_frag.a);
	vec4 shadow_color = vec4(0.2, 0.2, 0.2, shadow_frag.a);
	
	fragColor = mix(shadow_color, text_color, color_frag.a);
}