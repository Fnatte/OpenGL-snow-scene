#include <GL/glew.h>

#include "libraries/GLUtilities.h"

#include "full.h"
#include "utilities.h"


static GLuint positionLocation = 0;
static GLuint normalLocation   = 1;
static GLuint textureLocation  = 2;

static GLuint cameraLocation;
static GLuint modelLocation;
static GLuint shadowMapLocation;


void initializeFullShader(GLuint texUnit) {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");
	cameraLocation =	glGetUniformLocation(fullProgram, "camera");
	modelLocation =	glGetUniformLocation(fullProgram, "model");
	shadowMapLocation = glGetUniformLocation(fullProgram, "shadowMap");
	glUniform1i(glGetUniformLocation(fullProgram, "textureUnit"), texUnit);
}


void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4 shadowMapTransform) {
	mat4 mvpTransform = Mult(cameraTransform, modelTransform);
	mat4 shadowMapModelTransform = Mult(shadowMapTransform, modelTransform);

	glUseProgram(fullProgram);

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, mvpTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelTransform.m);
	glUniformMatrix4fv(shadowMapLocation, 1, GL_TRUE, shadowMapModelTransform.m);

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
