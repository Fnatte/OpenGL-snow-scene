#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 textureCoordinates;

uniform mat4 camera;
uniform mat4 model;

out vec2 fragTexCoord;

void main() {
	// Transform vertex to light source coordinates
	gl_Position = camera * model * vec4(inPosition, 1.0);

	// Pass to fragment shader
	fragTexCoord = textureCoordinates;
}
