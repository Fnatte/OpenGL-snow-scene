#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;

uniform mat4 shadowMapTransform;
uniform mat4 modelViewProjectionTransform;

out vec4 lightSourceCoord;

void main() {
	// Transform vertex to light source coordinates
	lightSourceCoord = shadowMapTransform * vec4(inPosition, 1.0);
	gl_Position = modelViewProjectionTransform * vec4(inPosition, 1.0);
}
