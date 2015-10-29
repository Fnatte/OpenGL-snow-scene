#version 150

in VERTEX
{
	vec3 normal;
	vec2 textureCoordinates;
} vertex;

uniform sampler2D texUnit;

out vec4 outColor;

void main(void){
	outColor = vec4(1, 1, 1, 1); // texture(texUnit, vertex.textureCoordinates);
}
