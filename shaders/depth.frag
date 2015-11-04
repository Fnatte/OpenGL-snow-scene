#version 150
#extension GL_ARB_explicit_attrib_location : enable

// layout(location = 0) out vec4 gl_ ;
out vec4 outColor;

void main(void) {
	// fragmentdepth = gl_FragCoord.z;
	outColor = vec4(1);
}
