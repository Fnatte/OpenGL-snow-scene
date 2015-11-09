#version 150

in vec3 inPosition;

uniform mat4 modelViewProjectionTransform;

void main(void) {
    gl_Position = modelViewProjectionTransform * vec4(inPosition, 1.0);
}
