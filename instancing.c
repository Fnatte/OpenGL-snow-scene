#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <assert.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/LoadObject.h"
#include "./libraries/VectorUtils3.h"

#include "utilities.h"
#include "light.h"


static GLuint instanceTransBuffer;

static GLuint positionLocation = 0;
static GLuint normalLocation   = 1;
//static GLuint textureLocation  = 2;
static GLuint instanceLocation  = 3;

static GLint cameraLocation;
static GLint modelLocation;
static GLint materialLocation;

static GLint lightPositionLocation;
static GLint lightIntensitiesLocation;
static GLint lightAttenuationLocation;
static GLint lightAmbientCoefficientLocation;
static GLint lightConeAngleLocation;
static GLint lightConeDirectionLocation;

static float* randoms;

static int count;

GLuint instancingProgram;

void initializeInstancingShader(int _count) {
	count = _count;
	randoms = getRandFloatArray(count * count * count, 1.0, 4.0);

	instancingProgram = loadShaders("./shaders/instancing.vert", "./shaders/instancing.frag");

	cameraLocation = glGetUniformLocation(instancingProgram, "camera");
	modelLocation = glGetUniformLocation(instancingProgram, "model");
	//materialLocation = glGetUniformLocation(instancingProgram, "material");

	lightPositionLocation = glGetUniformLocation(instancingProgram, "light.position");
	lightIntensitiesLocation = glGetUniformLocation(instancingProgram, "light.intensities");
	lightAttenuationLocation = glGetUniformLocation(instancingProgram, "light.attentuation");
	lightAmbientCoefficientLocation = glGetUniformLocation(instancingProgram, "light.ambientCoefficient");
	lightConeAngleLocation = glGetUniformLocation(instancingProgram, "light.coneAngle");
	lightConeDirectionLocation = glGetUniformLocation(instancingProgram, "light.coneDirection");

	assert(cameraLocation >= 0);
	assert(modelLocation >= 0);
	assert(materialLocation >= 0);
	assert(lightPositionLocation >= 0);

	glUseProgram(instancingProgram);
	glGenBuffers(1, &instanceTransBuffer);
	glUniform1i(materialLocation, 0);
}

static void setLightUniform(struct ShaderLight *light) {
	glUniform3f(lightPositionLocation, light->position.x, light->position.y, light->position.z);
	glUniform3f(lightIntensitiesLocation, light->intensities.x, light->intensities.y, light->intensities.z);
	glUniform3f(lightConeDirectionLocation, light->coneDirection.x, light->coneDirection.y, light->coneDirection.z);
	glUniform1f(lightAttenuationLocation, light->attenuation);
	glUniform1f(lightAmbientCoefficientLocation, light->ambientCoefficient);
	glUniform1f(lightConeAngleLocation, light->coneAngle);
}

static void createInstanceTransforms(mat4 *transforms, double time, mat4 modelTransform) {
	for (int x = 0; x < count; x++) {
		for (int y = 0; y < count; y++) {
			for (int z = 0; z < count; z++) {
				int index = x + y * count + z * count * count;
				float scale = (3 - randoms[index])/6;
				transforms[index] = Transpose(
						Mult(Mult(Mult(Mult(T(
													   x * 4 + randoms[index],
													   fmod(-(time * (randoms[index] + 12.0) + (float)index), 100),
													   z * 4 + randoms[index]
											   ), modelTransform),
									   Rx(time * (randoms[index] - 1.0))),
								  Rz(time * (randoms[index + 1] - 1.0))),
							 S(scale, scale, scale)
						)
				);
			}
		}
	}
}

void drawModelInstanced(Model *m, mat4 cameraTransform, mat4 modelTransform, struct Light* light) {
	double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	// Generate data.
	mat4 instanceTransforms[count * count * count];
	createInstanceTransforms(instanceTransforms, time, modelTransform);

	glUseProgram(instancingProgram);

	// Bind textures
//	glActiveTexture(GL_TEXTURE0 + 0);
//	glBindTexture(GL_TEXTURE_2D, texture);

	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, cameraTransform.m);
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, modelTransform.m);

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
//	glBindBuffer(GL_ARRAY_BUFFER, m->tb);
//	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(textureLocation);

	// Upload instance transforms
	glBindBuffer(GL_ARRAY_BUFFER, instanceTransBuffer);
	for (int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(instanceLocation + i);
		glVertexAttribPointer(instanceLocation + i,       // Location
							  4, GL_FLOAT, GL_FALSE,      // vec4
							  sizeof(mat4),               // Stride
							  (void*)(sizeof(vec4) * i)); // Start offset
		glVertexAttribDivisor(instanceLocation + i, 1);
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(instanceTransforms), &instanceTransforms, GL_STATIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L, count * count * count);
}
