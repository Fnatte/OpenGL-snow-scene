#version 150

in vec2 outTexCoord;
uniform sampler2D image;
out vec4 out_Color;

void main(void) {
    // vec4 a = texture(image, outTexCoord);
		// out_Color = vec4(a.x, a.y, 1, 1);
    // out_Color = texture(image, outTexCoord);
		
    float z = texture(image, outTexCoord).z;
		out_Color = vec4(z);
}
