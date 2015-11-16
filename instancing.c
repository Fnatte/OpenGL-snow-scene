#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/LoadObject.h"
#include "./libraries/LoadTGA.h"
#include "./libraries/VectorUtils3.h"

#include "main.h"
#include "utilities.h"
#include "light.h"


static GLuint positionLocation = 0;
static GLuint normalLocation   = 1;
static GLuint textureLocation  = 2;
static GLuint instanceTransformLocation  = 3;

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

static GLuint instanceTransBuffer;

float* randoms;

GLuint count;

GLuint instancingProgram;


void initializeInstancingShader(GLuint _count) {
	count = _count;

	instancingProgram = loadShaders("./shaders/instancing.vert", "./shaders/full.frag");

	cameraLocation = glGetUniformLocation(instancingProgram, "camera");
	modelLocation = glGetUniformLocation(instancingProgram, "model");
	shadowMapTransformLocation = glGetUniformLocation(instancingProgram, "shadowMapTransform");
	materialLocation = glGetUniformLocation(instancingProgram, "material");
	normalMapLocation = glGetUniformLocation(instancingProgram, "normalMap");
	shadowMapLocation = glGetUniformLocation(instancingProgram, "shadowMap");

	lightPositionLocation = glGetUniformLocation(instancingProgram, "light.position");
	lightIntensitiesLocation = glGetUniformLocation(instancingProgram, "light.intensities");
	lightAttenuationLocation = glGetUniformLocation(instancingProgram, "light.attentuation");
	lightAmbientCoefficientLocation = glGetUniformLocation(instancingProgram, "light.ambientCoefficient");
	lightConeAngleLocation = glGetUniformLocation(instancingProgram, "light.coneAngle");
	lightConeDirectionLocation = glGetUniformLocation(instancingProgram, "light.coneDirection");

	glUseProgram(instancingProgram);
	glUniform1i(materialLocation, 0);
	glUniform1i(normalMapLocation, 2);
	glUniform1i(shadowMapLocation, 4);

	glGenBuffers(1, &instanceTransBuffer);

	randoms = getRandFloatArray(count * count * count, 1.0, 4.0);
}

static void setLightUniform(struct ShaderLight *light) {
	glUniform3f(lightPositionLocation, light->position.x, light->position.y, light->position.z);
	glUniform3f(lightIntensitiesLocation, light->intensities.x, light->intensities.y, light->intensities.z);
	glUniform3f(lightConeDirectionLocation, light->coneDirection.x, light->coneDirection.y, light->coneDirection.z);
	glUniform1f(lightAttenuationLocation, light->attenuation);
	glUniform1f(lightAmbientCoefficientLocation, light->ambientCoefficient);
	glUniform1f(lightConeAngleLocation, light->coneAngle);
}


void drawModelInstanced(Model *m, mat4 cameraTransform, mat4 groupTransform, mat4 shadowMapTransform, GLuint texture,
						GLuint shadowMap, struct Light *light) {
	glUseProgram(instancingProgram);

	// Bind textures
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	// Set light uniform
	struct ShaderLight shaderLight = getShaderLight(light);
	setLightUniform(&shaderLight);

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, cameraTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, groupTransform.m);
	glUniformMatrix4fv(shadowMapTransformLocation, 1, GL_TRUE, shadowMapTransform.m);

	GLfloat time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	glBindVertexArray(m->vao);

	// Generate data.
	mat4 instanceTransforms[count * count * count];
	for (GLuint x = 0; x < count; x++) {
		for (GLuint y = 0; y < count; y++) {
			for (GLuint z = 0; z < count; z++) {
				int pos = x + y * count + z * count * count;
				instanceTransforms[pos] =
						Mult(Mult(Mult(Mult(T(x * 4 + randoms[pos],
											  fmod(-(time * (randoms[pos] + 12.0) + (float) pos), 100),
											  z * 4 + randoms[pos]), groupTransform),
									   Rx(time * (randoms[pos] - 1.0))),
								  Rz(time * (randoms[pos + 1] - 1.0))),
							 S((6 - randoms[pos]) / 4, (6 - randoms[pos]) / 4, (6 - randoms[pos]) / 4));
				instanceTransforms[pos] = Transpose(instanceTransforms[pos]);
			}
		}
	}

	// Prepare shader data fields and upload to them.
	glBindBuffer(GL_ARRAY_BUFFER, instanceTransBuffer);
	for (int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(instanceTransformLocation + i);
		glVertexAttribPointer(instanceTransformLocation + i,             // Location
		                      4, GL_FLOAT, GL_FALSE,      // vec4
		                      sizeof(mat4),               // Stride
		                      (void*)(sizeof(vec4) * i)); // Start offset
		glVertexAttribDivisor(instanceTransformLocation + i, 1);
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(instanceTransforms), &instanceTransforms, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m->vb);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLocation);

	if (m->texCoordArray != NULL) {
		glBindBuffer(GL_ARRAY_BUFFER, m->tb);
		glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(textureLocation);
	}

	glDrawElementsInstanced(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L, count * count * count);
}
