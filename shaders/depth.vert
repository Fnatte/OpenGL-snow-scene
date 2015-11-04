#version 150
#extension GL_ARB_explicit_attrib_location : enable

in vec3 in_Position;
uniform mat4 projectionViewWorldTransform;

void main(void) {
	gl_Position = projectionViewWorldTransform * vec4(in_Position, 1.0);
}
