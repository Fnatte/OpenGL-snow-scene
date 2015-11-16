#ifndef LIGHT_H
#define LIGHT_H

#include "camera.h"
#include "./libraries/VectorUtils3.h"

struct Light {
	struct Camera camera;
	vec3 intensities;
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
};

struct ShaderLight {
	vec3 position;
	vec3 intensities;
	vec3 coneDirection;
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
};

struct ShaderLight getShaderLight(struct Light *light);

#endif