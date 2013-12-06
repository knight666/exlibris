#version 330 core

in vec2 attrPosition;
in vec2 attrTextureCoordinate0;

out vec2 vertTextureCoordinate;

uniform mat4 matModelViewProjection;

void main()
{
	gl_Position = matModelViewProjection * vec4(attrPosition, 0.0, 1.0);
	vertTextureCoordinate = attrTextureCoordinate0;
}