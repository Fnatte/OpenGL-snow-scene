#include <GL/glew.h>

#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"
#include "libraries/GLUtilities.h"

#include "skybox.h"
#include "content.h"
#include "main.h"


static GLuint projectionViewLocation;
static GLuint transformLocation;

static GLuint skyboxProgram;

void initializeSkyboxShader() {
	skyboxProgram = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	glUseProgram(skyboxProgram);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxProgram, "texUnit"), 0);
	projectionViewLocation = glGetUniformLocation(skyboxProgram, "projectionViewMatrix");
	transformLocation = glGetUniformLocation(skyboxProgram, "transform");
}


void drawSkybox(mat4 transform) {
	mat4 cameraTrans = T(userCamera.position.x, userCamera.position.y, userCamera.position.z);

	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(projectionViewLocation, 1, GL_TRUE, transform.m);
	glBindTexture(GL_TEXTURE_2D, textureSkybox);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(transformLocation, 1, GL_TRUE, cameraTrans.m);
	DrawModel(modelSkybox, skyboxProgram, "in_Position", NULL, "in_TexCoord");
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
