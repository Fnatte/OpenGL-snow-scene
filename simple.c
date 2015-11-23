#include <GL/glew.h>

#include "libraries/GLUtilities.h"

#include "simple.h"

static GLuint positionLocation = 0;
static GLuint textureLocation  = 1;

static GLint cameraLocation;
static GLint modelLocation;

static GLint materialLocation;

void initializeSimpleShader() {
	simpleProgram = loadShaders("shaders/simple.vert", "shaders/simple.frag");

	cameraLocation = glGetUniformLocation(simpleProgram, "camera");
	modelLocation = glGetUniformLocation(simpleProgram, "model");
	materialLocation = glGetUniformLocation(simpleProgram, "material");

	glUseProgram(simpleProgram);
	glUniform1i(materialLocation, 0);
}

void drawSimple(Model *m, mat4 cameraTransform, mat4 modelTransform, GLuint texture) {
	// Bind textures
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUseProgram(simpleProgram);

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, cameraTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelTransform.m);

	// Vertex positions.
	glBindVertexArray(m->vao);
	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);

	// Texture coordinates.
	glBindBuffer(GL_ARRAY_BUFFER, m->tb);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureLocation);

	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
}
