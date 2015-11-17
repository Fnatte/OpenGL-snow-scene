#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "utilities.h"

void drawModelWithLocations(Model *m, GLuint positionLoc, GLuint normalLoc, GLuint textureLoc)
{
	glBindVertexArray(m->vao);

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLoc);

	// Normal
	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLoc);

	// Texture
	glBindBuffer(GL_ARRAY_BUFFER, m->tb);
	glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureLoc);

	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
}


float* getRandFloatArray(int size, float upperLimit, float lowerLimit) {
	float* randoms = malloc(size * sizeof(float));
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		randoms[i] = (float)rand() / (float)(RAND_MAX / (upperLimit - lowerLimit)) + lowerLimit;
	}
	return randoms;
}

vec3 getRandVec3(float upperLimit, float lowerLimit) {
	float divider = (float)(RAND_MAX / (upperLimit - lowerLimit)) + lowerLimit;

	return SetVector(
		(float)rand() / divider,
		(float)rand() / divider,
		(float)rand() / divider
	);
}


void printVec3(vec3 in) {
	printf("(%f, %f, %f)\n", in.x, in.y, in.z);
}
