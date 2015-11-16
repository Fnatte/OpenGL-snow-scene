#ifndef INSTANCING_H
#define INSTANCING_H

#include "light.h"

void initializeInstancingShader(GLuint _count);
void drawModelInstanced(Model *m, mat4 cameraTransform, mat4 groupTransform, mat4 shadowMapTransform, GLuint texture,
						GLuint shadowMap, struct Light *light);

#endif