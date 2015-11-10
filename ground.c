#include <GL/glew.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/LoadObject.h"
#include "./libraries/LoadTGA.h"
#include "./libraries/VectorUtils3.h"

#include "./utilities.h"
#include "./content.h"

GLuint positionLocation = 0;
GLuint normalLocation   = 1;
GLuint textureLocation  = 2;

GLuint modelViewProjectionLocation;
GLuint shadowMapLocation;

Model *model;

GLuint groundTexture;

void initializeGround(Model *_model, GLuint program) {
	model = _model;
	modelViewProjectionLocation =	glGetUniformLocation(program, "modelViewProjectionTransform");
	shadowMapLocation = glGetUniformLocation(program, "shadowMapTransform");
}


void drawGroundWithProgram(GLuint program, mat4 modelViewProjectionTransform, mat4 shadowMapTransform) {
	glUseProgram(program);
	glUniform1f(glGetUniformLocation(program, "shade"), 0.3);
	glUniformMatrix4fv(shadowMapLocation, 1, GL_TRUE, shadowMapTransform.m);
	glUniformMatrix4fv(modelViewProjectionLocation, 1, GL_TRUE, modelViewProjectionTransform.m);

	// Uncommenting this disables shadows.
	// glBindTexture(GL_TEXTURE_2D, textureGroundSpecular);
	drawModelWithLocations(model, positionLocation, normalLocation, textureLocation);
}
