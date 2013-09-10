#version 330 core

in vec2 attrPosition;

uniform mat4 matModelViewProjection;
uniform float uniTime;
uniform vec2 uniOffset;

void main()
{	
	float time = (cos(uniTime) * 0.5 + 0.5);
	float scale = 1.0 - (position.x / 1024.0);
	vec2 scale_both = vec2(1.0, time * 3.5 + 1.0);
	
	vec2 position = attrPosition * scale_both;
	
	gl_Position = matModelViewProjection * vec4(position * scale_both + uniOffset, 0.0, 1.0);
}	