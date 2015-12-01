#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in mat4 instanceTrans; // Per instance transformation. Consumes location 3, 4, 5 and 6.

uniform mat4 camera;
uniform mat4 model;

//out vec2 fragTexCoord;
out vec3 fragPosition;
out vec3 fragNormal;
out mat4 fragInstanceTrans;

void main(void)
{
	gl_Position = camera * model * instanceTrans * vec4(position, 1.0);

	// Pass to fragment shader
//	fragTexCoord = textureCoordinates;
	fragPosition = position;
	fragNormal = normal;
	fragInstanceTrans = instanceTrans;
}
