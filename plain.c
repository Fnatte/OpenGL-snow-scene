#include <GL/glew.h>
#include <assert.h>

#include "plain.h"
#include "utilities.h"


static GLuint positionLocation = 0;

static GLuint modelViewProjectionLocation;


void initializePlainShader() {
	plainProgram = loadShaders("shaders/plain.vert", "shaders/plain.frag");

	modelViewProjectionLocation = glGetUniformLocation(plainProgram, "modelViewProjectionTransform");

	assert(modelViewProjectionLocation >= 0);
}


void drawPlain(Model *m, mat4 modelViewProjectionTransform, mat4 modelTransform) {
	modelViewProjectionTransform = Mult(modelViewProjectionTransform, modelTransform);

	glUseProgram(plainProgram);
	glUniformMatrix4fv(modelViewProjectionLocation, 1, GL_TRUE, modelViewProjectionTransform.m);

	// Vertex positions.
	glBindVertexArray(m->vao);
	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);

	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	printError("drawPlain()");
}
