#include "light.h"

struct ShaderLight getShaderLight(struct Light *light) {
	return (struct ShaderLight) {
		.position = light->camera.position,
		.intensities = light->intensities,
		.coneDirection = VectorSub(light->camera.target, light->camera.position),
		.coneAngle = light->coneAngle,
		.attenuation = light->attenuation,
		.ambientCoefficient = light->ambientCoefficient
	};
}