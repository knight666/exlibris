#version 330 core

in vec2 attrPosition;

uniform mat4 matModelViewProjection;
uniform float uniTime;
uniform vec2 uniTextDimensions;
uniform vec2 uniOffset;

vec2 GetOffsetBezier(float a_Time)
{
	vec2 a = vec2(0.0, 70.0);
	vec2 b = vec2(0.0, 25.0);
	vec2 c = vec2(0.0, 50.0);
	
	vec2 ac = mix(a, c, a_Time);
	vec2 cb = mix(c, b, a_Time);
	vec2 mixed = mix(ac, cb, a_Time);
	
	return mixed;
}

void main()
{	
	float time = (cos(uniTime) * 0.5 + 0.5);
	//vec2 scale_both = vec2(time * 0.25 + 1.0, time * 0.55 + 1.0);
	
	float distance = attrPosition.x / uniTextDimensions.x;
	
	//vec2 scale_both = vec2(1.0, 1.0 + (1.0 - distance) * time);
	vec2 scale_both = vec2(1.0, 1.0 + ((1.0 - distance) * 2.5));
	
	//vec2 offset = uniOffset + vec2(0.0, cos(distance * 5.0) * 10.0);
	vec2 offset = uniOffset + GetOffsetBezier(distance);
	//vec2 offset = uniOffset;
	
	vec4 position = vec4(attrPosition * scale_both + offset, 0.0, 1.0);
	
	gl_Position = matModelViewProjection * position;
}	