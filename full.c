#include <GL/glew.h>
#include <assert.h>

#include "libraries/GLUtilities.h"

#include "full.h"

static GLuint positionLocation = 0;
static GLuint normalLocation   = 1;
static GLuint textureLocation  = 2;

static GLint cameraLocation;
static GLint modelLocation;
static GLint shadowMapTransformLocation;

static GLint materialLocation;
static GLint normalMapLocation;
static GLint shadowMapLocation;

static GLint lightPositionLocation;
static GLint lightIntensitiesLocation;
static GLint lightAttenuationLocation;
static GLint lightAmbientCoefficientLocation;
static GLint lightConeAngleLocation;
static GLint lightConeDirectionLocation;

static struct Light *light;

void initializeFullShader() {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");

	cameraLocation = glGetUniformLocation(fullProgram, "camera");
	modelLocation = glGetUniformLocation(fullProgram, "model");
	shadowMapTransformLocation = glGetUniformLocation(fullProgram, "shadowMapTransform");
	materialLocation = glGetUniformLocation(fullProgram, "material");
	normalMapLocation = glGetUniformLocation(fullProgram, "normalMap");
	shadowMapLocation = glGetUniformLocation(fullProgram, "shadowMap");

	lightPositionLocation = glGetUniformLocation(fullProgram, "light.position");
	lightIntensitiesLocation = glGetUniformLocation(fullProgram, "light.intensities");
	lightAttenuationLocation = glGetUniformLocation(fullProgram, "light.attentuation");
	lightAmbientCoefficientLocation = glGetUniformLocation(fullProgram, "light.ambientCoefficient");
	lightConeAngleLocation = glGetUniformLocation(fullProgram, "light.coneAngle");
	lightConeDirectionLocation = glGetUniformLocation(fullProgram, "light.coneDirection");

	assert(cameraLocation >= 0);
	assert(modelLocation >= 0);
	// assert(shadowMapTransformLocation >= 0);
	assert(materialLocation >= 0);
	// assert(normalMapLocation >= 0);
	// assert(shadowMapLocation >= 0);

	glUseProgram(fullProgram);
	glUniform1i(materialLocation, 0);
	glUniform1i(normalMapLocation, 2);
	glUniform1i(shadowMapLocation, 4);
}

static void setLightUniform(struct ShaderLight *light) {
	glUniform3f(lightPositionLocation, light->position.x, light->position.y, light->position.z);
	glUniform3f(lightIntensitiesLocation, light->intensities.x, light->intensities.y, light->intensities.z);
	glUniform3f(lightConeDirectionLocation, light->coneDirection.x, light->coneDirection.y, light->coneDirection.z);
	glUniform1f(lightAttenuationLocation, light->attenuation);
	glUniform1f(lightAmbientCoefficientLocation, light->ambientCoefficient);
	glUniform1f(lightConeAngleLocation, light->coneAngle);
}

void setLight(struct Light *_light) {
	light = _light;
}

void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4 shadowMapTransform, GLuint texture, GLuint shadowMap) {
	mat4 mvpTransform = Mult(cameraTransform, modelTransform);
	mat4 shadowMapModelTransform = Mult(shadowMapTransform, modelTransform);

	// Bind textures
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUseProgram(fullProgram);

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, mvpTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelTransform.m);
	glUniformMatrix4fv(shadowMapTransformLocation, 1, GL_TRUE, shadowMapModelTransform.m);

	// Set light uniform
	struct ShaderLight shaderLight = getShaderLight(light);
	setLightUniform(&shaderLight);

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
