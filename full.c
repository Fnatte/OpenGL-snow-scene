#include <GL/glew.h>
#include <assert.h>

#include "libraries/GLUtilities.h"

#include "full.h"
#include "utilities.h"


static GLint positionLocation = 0;
static GLint normalLocation   = 1;
static GLint textureLocation  = 2;

static GLint modelViewProjectionLocation;
static GLint shadowMapTransformLocation;

static GLint materialLocation;
static GLint normalMapLocation;
static GLint shadowMapLocation;


void initializeFullShader() {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");

	modelViewProjectionLocation = glGetUniformLocation(fullProgram, "modelViewProjectionTransform");
	shadowMapTransformLocation = glGetUniformLocation(fullProgram, "shadowMapTransform");
	materialLocation = glGetUniformLocation(fullProgram, "material");
	normalMapLocation = glGetUniformLocation(fullProgram, "normalMap");
	shadowMapLocation = glGetUniformLocation(fullProgram, "shadowMap");

	assert(modelViewProjectionLocation >= 0);
	assert(shadowMapTransformLocation >= 0);
	assert(textureLocation >= 0);
	// assert(materialLocation >= 0);
	// assert(normalMapLocation >= 0);
	assert(shadowMapLocation >= 0);

	glUseProgram(fullProgram);
	glUniform1i(materialLocation, 0);
	glUniform1i(normalMapLocation, 2);
	glUniform1i(shadowMapLocation, 4);
}


void drawFull(Model *m, mat4 modelViewProjectionTransform, mat4 shadowMapTransform, mat4 modelTransform, GLuint texture, GLuint shadowMap) {
	modelViewProjectionTransform = Mult(modelViewProjectionTransform, modelTransform);
	shadowMapTransform = Mult(shadowMapTransform, modelTransform);

	// Bind textures
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUseProgram(fullProgram);

	glUniformMatrix4fv(modelViewProjectionLocation, 1, GL_TRUE, modelViewProjectionTransform.m);
	glUniformMatrix4fv(shadowMapTransformLocation, 1, GL_TRUE, shadowMapTransform.m);

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
