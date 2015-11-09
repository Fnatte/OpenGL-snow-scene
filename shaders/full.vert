#version 150

in vec3 in_Position;

uniform int texunit;
uniform mat4 shadowMapTransform;
uniform mat4 modelViewProjectionTransform;

out vec4 lightSourceCoord;

void main() {
	// Transform vertex to light source coordinates
	lightSourceCoord = shadowMapTransform * vec4(in_Position, 1.0);
	gl_Position = modelViewProjectionTransform * vec4(in_Position, 1.0);
}
