#include <GL/glew.h>

#include "libraries/GLUtilities.h"

#include "full.h"
#include "utilities.h"


static GLuint positionLocation = 0;
static GLuint normalLocation   = 1;
static GLuint textureLocation  = 2;

static GLuint modelViewProjectionLocation;
static GLuint shadowMapLocation;
static GLuint shadeLocation;


void initializeFullShader(GLuint texUnit) {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");
	modelViewProjectionLocation =	glGetUniformLocation(fullProgram, "modelViewProjectionTransform");
	shadowMapLocation = glGetUniformLocation(fullProgram, "shadowMapTransform");
	shadeLocation = glGetUniformLocation(fullProgram, "shade");
	glUniform1i(glGetUniformLocation(fullProgram, "textureUnit"), texUnit);
}


void drawFull(Model *m, mat4 modelViewProjectionTransform, mat4 shadowMapTransform, mat4 modelTransform, GLfloat shade) {
	modelViewProjectionTransform = Mult(modelViewProjectionTransform, modelTransform);
	shadowMapTransform = Mult(shadowMapTransform, modelTransform);

	glUseProgram(fullProgram);

	glUniformMatrix4fv(modelViewProjectionLocation, 1, GL_TRUE, modelViewProjectionTransform.m);
		glUniformMatrix4fv(shadowMapLocation, 1, GL_TRUE, shadowMapTransform.m);
	glUniform1f(shadeLocation, shade);

	// Vertex positions.
	glBindVertexArray(m->vao);
	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLocation);

	// Texture coordinates.
	glBindBuffer(GL_ARRAY_BUFFER, m->tb);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureLocation);

	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
}
