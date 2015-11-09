#version 150

in vec3 inPosition;

uniform mat4 shadowMapTransform;
uniform mat4 modelViewProjectionTransform;

out vec4 lightSourceCoord;

void main() {
	// Transform vertex to light source coordinates
	lightSourceCoord = shadowMapTransform * vec4(inPosition, 1.0);
	gl_Position = modelViewProjectionTransform * vec4(inPosition, 1.0);
}
