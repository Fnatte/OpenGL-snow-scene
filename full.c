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
static GLint shadowMapTransformLocation[MAX_LIGHTS];
static GLint cameraPositionLocation;

static GLint materialLocation;
static GLint normalMapLocation;
static GLint shadowMapLocation[MAX_LIGHTS];

static GLint nrLightsLocation;
static GLint lightPositionLocation[MAX_LIGHTS];
static GLint lightIntensitiesLocation[MAX_LIGHTS];
static GLint lightAmbientCoefficientLocation[MAX_LIGHTS];
static GLint lightConeAngleLocation[MAX_LIGHTS];
static GLint lightConeDirectionLocation[MAX_LIGHTS];


void initializeFullShader() {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");
	glUseProgram(fullProgram);

	cameraPositionLocation = glGetUniformLocation(fullProgram, "cameraPosition");
	assert(cameraPositionLocation >= 0);
	cameraLocation = glGetUniformLocation(fullProgram, "camera");
	assert(cameraLocation >= 0);
	modelLocation = glGetUniformLocation(fullProgram, "model");
	assert(modelLocation >= 0);
	materialLocation = glGetUniformLocation(fullProgram, "material");
	assert(materialLocation >= 0);
	normalMapLocation = glGetUniformLocation(fullProgram, "normalMap");
	// assert(normalMapLocation >= 0); Not used yet
	nrLightsLocation = glGetUniformLocation(fullProgram, "nrLights");
	assert(nrLightsLocation >= 0);

	char name[100];
	for (int i = 0; i < MAX_LIGHTS; i++) {
		sprintf(name, "light[%d].position", i);
		lightPositionLocation[i] = glGetUniformLocation(fullProgram, name);
		assert(lightPositionLocation[i] >= 0);

		sprintf(name, "light[%d].intensities", i);
		lightIntensitiesLocation[i] = glGetUniformLocation(fullProgram, name);
		assert(lightIntensitiesLocation[i] >= 0);

		sprintf(name, "light[%d].ambientCoefficient", i);
		lightAmbientCoefficientLocation[i] = glGetUniformLocation(fullProgram, name);
		assert(lightAmbientCoefficientLocation[i] >= 0);

		sprintf(name, "light[%d].coneAngle", i);
		lightConeAngleLocation[i] = glGetUniformLocation(fullProgram, name);
		assert(lightConeAngleLocation[i] >= 0);

		sprintf(name, "light[%d].coneDirection", i);
		lightConeDirectionLocation[i] = glGetUniformLocation(fullProgram, name);
		assert(lightConeDirectionLocation[i] >= 0);

		sprintf(name, "shadowMap[%d]", i);
		shadowMapLocation[i] = glGetUniformLocation(fullProgram, name);
		glUniform1i(shadowMapLocation[i], 4 + i);
		assert(shadowMapLocation[i] >= 0);

		sprintf(name, "shadowMapTransform[%d]", i);
		shadowMapTransformLocation[i] = glGetUniformLocation(fullProgram, name);
		assert(shadowMapTransformLocation[i] >= 0);
	}

	glUniform1i(materialLocation, 0);
	glUniform1i(normalMapLocation, 2);
}

static void setLightUniform(struct ShaderLight *light, int index) {
	glUniform3f(lightPositionLocation[index], light->position.x, light->position.y, light->position.z);
	glUniform3f(lightIntensitiesLocation[index], light->intensities.x, light->intensities.y, light->intensities.z);
	glUniform3f(lightConeDirectionLocation[index], light->coneDirection.x, light->coneDirection.y, light->coneDirection.z);
	glUniform1f(lightAmbientCoefficientLocation[index], light->ambientCoefficient);
	glUniform1f(lightConeAngleLocation[index], light->coneAngle);
}

void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4* shadowMapTransforms, GLuint texture,
              FBOstruct** shadowMaps, struct StreetLight* lights, int nrLights, vec3 cameraPosition) {

	// Bind textures
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	for (int i = 0; i < nrLights; i++) {
		glActiveTexture(GL_TEXTURE0 + 4 + i);
		glBindTexture(GL_TEXTURE_2D, shadowMaps[i]->depth);
	}

	glUseProgram(fullProgram);

	glUniform3f(cameraPositionLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, cameraTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelTransform.m);

	glUniform1i(nrLightsLocation, nrLights);
	// Set light uniforms.
	for (int i = 0; i < nrLights; i++) {
		glUniformMatrix4fv(shadowMapTransformLocation[i], 1, GL_TRUE, Mult(shadowMapTransforms[i], modelTransform).m);
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
