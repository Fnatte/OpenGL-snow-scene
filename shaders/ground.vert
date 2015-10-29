#version 150
#extension GL_ARB_explicit_attrib_location : enable

// "position" and "normal" are regular vertex attributes
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

// Color is a per-instance attribute
layout (location = 2) in vec2 textureCoordinates;

// uniform mat4 lookMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// The output of the vertex shader (matched to thead// fragment shader)
out VERTEX
{
	vec3 normal;
	vec2 textureCoordinates;
} vertex;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * position;
	vertex.normal =  normal;
	vertex.textureCoordinates = textureCoordinates;
}
