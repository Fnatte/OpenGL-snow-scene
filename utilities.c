#include <GL/glew.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "./libraries/LoadObject.h"


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
