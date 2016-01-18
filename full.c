#include <GL/glew.h>
#include <assert.h>
#include <stdio.h>

#include "libraries/GLUtilities.h"

#include "full.h"

static GLuint positionLocation = 0;
static GLuint normalLocation   = 1;
static GLuint textureLocation  = 2;

static GLint cameraLocation;
static GLint modelLocation;
static GLint shadowMapTransformLocation;
static GLint cameraPositionLocation;

static GLint materialLocation;
static GLint normalMapLocation;
static GLint shadowMapLocation;

static GLint nrLightsLocation;
static GLint lightPositionLocation[MAX_LIGHTS];
static GLint lightIntensitiesLocation[MAX_LIGHTS];
static GLint lightAmbientCoefficientLocation[MAX_LIGHTS];
static GLint lightConeAngleLocation[MAX_LIGHTS];
static GLint lightConeDirectionLocation[MAX_LIGHTS];



void initializeFullShader() {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");

	cameraPositionLocation = glGetUniformLocation(fullProgram, "cameraPosition");
	cameraLocation = glGetUniformLocation(fullProgram, "camera");
	modelLocation = glGetUniformLocation(fullProgram, "model");
	shadowMapTransformLocation = glGetUniformLocation(fullProgram, "shadowMapTransform");
	materialLocation = glGetUniformLocation(fullProgram, "material");
	normalMapLocation = glGetUniformLocation(fullProgram, "normalMap");
	shadowMapLocation = glGetUniformLocation(fullProgram, "shadowMap");
	nrLightsLocation = glGetUniformLocation(fullProgram, "nrLights");

	char name[100];
	for (int i = 0; i < MAX_LIGHTS; i++) {
		sprintf(name, "light[%d].position", i);
		lightPositionLocation[i] = glGetUniformLocation(fullProgram, name);
		sprintf(name, "light[%d].intensities", i);
		lightIntensitiesLocation[i] = glGetUniformLocation(fullProgram, name);
		sprintf(name, "light[%d].ambientCoefficient", i);
		lightAmbientCoefficientLocation[i] = glGetUniformLocation(fullProgram, name);
		sprintf(name, "light[%d].coneAngle", i);
		lightConeAngleLocation[i] = glGetUniformLocation(fullProgram, name);
		sprintf(name, "light[%d].coneDirection", i);
		lightConeDirectionLocation[i] = glGetUniformLocation(fullProgram, name);
	}

	assert(cameraLocation >= 0);
	assert(modelLocation >= 0);
	assert(shadowMapTransformLocation >= 0);
	assert(materialLocation >= 0);
	// assert(normalMapLocation >= 0); Not used yet
	assert(shadowMapLocation >= 0);
	assert(nrLightsLocation >= 0);

	glUseProgram(fullProgram);
	glUniform1i(materialLocation, 0);
	glUniform1i(normalMapLocation, 2);
	glUniform1i(shadowMapLocation, 4);
}

static void setLightUniform(struct ShaderLight *light, int index) {
	glUniform3f(lightPositionLocation[index], light->position.x, light->position.y, light->position.z);
	glUniform3f(lightIntensitiesLocation[index], light->intensities.x, light->intensities.y, light->intensities.z);
	glUniform3f(lightConeDirectionLocation[index], light->coneDirection.x, light->coneDirection.y, light->coneDirection.z);
	glUniform1f(lightAmbientCoefficientLocation[index], light->ambientCoefficient);
	glUniform1f(lightConeAngleLocation[index], light->coneAngle);
}

void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4 shadowMapTransform, GLuint texture,
              GLuint shadowMap, struct StreetLight* lights, int nrLights, vec3 cameraPosition) {
	mat4 shadowMapModelTransform = Mult(shadowMapTransform, modelTransform);

	// Bind textures
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUseProgram(fullProgram);

	glUniform3f(cameraPositionLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, cameraTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelTransform.m);
	glUniformMatrix4fv(shadowMapTransformLocation, 1, GL_TRUE, shadowMapModelTransform.m);

	glUniform1i(nrLightsLocation, nrLights);
	// Set light uniforms.
	for (int i = 0; i < nrLights; i++) {
		struct ShaderLight shaderLight = getShaderLight(&lights[i].lamp);
		setLightUniform(&shaderLight, i);
	}

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
