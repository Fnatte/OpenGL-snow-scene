#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;

uniform mat4 projectionViewWorldTransform;

out VERTEX {
	vec3 normal;
	vec2 textureCoordinates;
} vertex;

void main(void) {
	gl_Position = projectionViewWorldTransform * position;
	vertex.normal =  normal;
	vertex.textureCoordinates = textureCoordinates;
}
