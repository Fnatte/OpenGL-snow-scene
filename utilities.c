#include <GL/gl.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "./libraries/LoadObject.h"

void drawModelWithLocations(Model *m, GLuint positionLocation, GLuint normalLocation, GLuint textureLocation)
{
	glBindVertexArray(m->vao);

	// Position	
	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);
	
	// Normal
	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLocation);
	
	// Texture
	glBindBuffer(GL_ARRAY_BUFFER, m->tb);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureLocation);
	
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

float* getRandFloatArray(int size, float upperLimit, float lowerLimit) {
	float* randoms = malloc(size * sizeof(float));
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		randoms[i] = (float)rand() / (float)(RAND_MAX / (upperLimit - lowerLimit)) + lowerLimit;
	}
	return randoms;
}
