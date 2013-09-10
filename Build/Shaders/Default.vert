#version 330 core

in vec2 attrPosition;

uniform mat4 matModelViewProjection;
uniform float uniTime;
uniform vec2 uniOffset;

void main()
{	
	float time = (cos(uniTime) * 0.5 + 0.5);
	vec2 scale_both = vec2(time * 0.25 + 1.0, time * 0.55 + 1.0);
	
	vec4 position = vec4(attrPosition * scale_both + uniOffset, 0.0, 1.0);
	
	gl_Position = matModelViewProjection * position;
}	