#ifndef LIGHT_H
#define LIGHT_H

#include "./libraries/VectorUtils3.h"

struct Light {
	vec3 position;
	vec3 intensities;
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
	vec3 coneDirection;
};

#endif