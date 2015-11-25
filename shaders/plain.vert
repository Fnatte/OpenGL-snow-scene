#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 inPosition;

uniform mat4 modelViewProjectionTransform;

void main(void) {
    gl_Position = modelViewProjectionTransform * vec4(inPosition, 1.0);
}
