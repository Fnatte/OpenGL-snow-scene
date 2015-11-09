#version 150

in vec3 in_Position;

uniform mat4 modelViewProjectionTransform;

void main(void) {
    gl_Position = modelViewProjectionTransform * vec4(in_Position, 1.0);
}
