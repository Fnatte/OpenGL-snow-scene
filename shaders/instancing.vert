#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in mat4 instanceTransform;

uniform mat4 shadowMapTransform;
uniform mat4 camera;
uniform mat4 model;

out vec4 lightSourceCoord;
out vec2 fragTexCoord;
out vec3 fragPosition;
out vec3 fragNormal;

void main() {
	// Transform vertex to light source coordinates
	lightSourceCoord = shadowMapTransform * model * vec4(inPosition, 1.0);
	gl_Position = camera * model * instanceTransform * vec4(inPosition, 1.0);

	// Pass to fragment shader
	fragTexCoord = textureCoordinates;
	fragPosition = inPosition;
	fragNormal = normal;
}
