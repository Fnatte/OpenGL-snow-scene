#include <GL/glew.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/LoadObject.h"
#include "./libraries/LoadTGA.h"
#include "./libraries/VectorUtils3.h"

#include "./utilities.h"

GLuint positionLocation = 0;
GLuint normalLocation   = 1;
GLuint textureLocation  = 2;

GLuint program;
GLuint textureDiffuse;
GLuint textureSpecular;
GLuint textureNormal;
Model *model;

GLuint groundTexture;

void initializeGround(Model *_model, GLuint _program) {
	model = _model;
	program = _program;

	LoadTGATextureSimple("./textures/ground/diffuse.tga", &textureDiffuse);
	LoadTGATextureSimple("./textures/ground/specular.tga", &textureSpecular);
	LoadTGATextureSimple("./textures/ground/normal.tga", &textureNormal);
}

void drawGround(mat4 matrix) {
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionViewWorldTransform"), 1, GL_TRUE, matrix.m);
	drawModelWithLocations(model, positionLocation, normalLocation, textureLocation);
}
