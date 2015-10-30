#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/LoadObject.h"
#include "./libraries/LoadTGA.h"
#include "./libraries/VectorUtils3.h"

#include "main.h"
#include "utilities.h"

GLuint modelMatrixBuffer;
GLuint testBuffer;

GLuint positionLoc = 0;
GLuint normalLoc   = 1;
GLuint textureLoc  = 2;
GLuint testLoc     = 3;
GLuint matrixLoc   = 4;

float* randoms;
float* randoms2;

void setupInstancedVertexAttributes(GLuint prog, int count) {
	glUseProgram(prog);
	glGenBuffers(1, &modelMatrixBuffer);
	glGenBuffers(1, &testBuffer);

	randoms = getRandFloatArray(count * count * count, 1.0, 4.0);
	randoms2 = getRandFloatArray(count * count * count, 1.0, 4.0);
}

void drawModelInstanced(Model *m, GLuint program, GLuint count, GLfloat time, mat4 transEverything) {
	glUseProgram(program);
	if (m != NULL)
		glBindVertexArray(m->vao);
	else {
		printf("Warning warning, fuckup in drawmodelinstanced");
		return;
	}

	// Generate data.
	mat4 modelMatrixes[count * count * count];
	vec3 testData[count * count * count];
	for (GLuint x = 0; x < count; x++) {
		for (GLuint y = 0; y < count; y++) {
			for (GLuint z = 0; z < count; z++) {
				int pos = x + y * count + z * count * count;
				modelMatrixes[pos] = Mult(Mult(Mult(T(x * 4 + randoms[pos],
				                                      fmod(-(time * (randoms[pos] + 10.0) + (float)pos),  100),
				                                      z * 4 + randoms2[pos]), transEverything),
				                               Rx(time * (randoms[pos + 1] - 5.0))),
				                          Rz(time * (randoms2[pos] - 5)));
				modelMatrixes[pos] = Transpose(modelMatrixes[pos]);
				testData[pos] = (vec3)
					{ (float)x / (float)count, (float)y / (float)count, (float)z / (float)count };
			}
		}
	}

	// Prepare shader data fields and upload to them.
	glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBuffer);
	for (int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(matrixLoc + i);
		glVertexAttribPointer(matrixLoc + i,             // Location
		                      4, GL_FLOAT, GL_FALSE,      // vec4
		                      sizeof(mat4),               // Stride
		                      (void*)(sizeof(vec4) * i)); // Start offset
		glVertexAttribDivisor(matrixLoc + i, 1);
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelMatrixes), &modelMatrixes, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, testBuffer);
	glVertexAttribPointer(testLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testData), testData, GL_STATIC_DRAW);
	glVertexAttribDivisor(testLoc, 1);
	glEnableVertexAttribArray(testLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLoc);

	if (m->texCoordArray != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, m->tb);
		glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(textureLoc);
	}

	glDrawElementsInstanced(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L, count * count * count);
}
