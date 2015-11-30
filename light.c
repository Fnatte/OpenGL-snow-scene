#include "libraries/LoadObject.h"

#include "light.h"
#include "content.h"


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


struct StreetLight createStreetLight(vec3 position) {
	vec3 lightPosition = (vec3) {
		position.x, position.y + 18.5, position.z - 1.67790 };
	vec3 lightTarget = (vec3) {
		position.x + 0.287315, position.y, position.z - 8.340088 };
	vec3 lightNormal = CrossProduct(lightPosition, lightTarget);

	struct Light lightSource = (struct Light) {
		.camera = createCamera(lightPosition, lightNormal, lightTarget),
		.intensities = (vec3){1.0f, 1.0f, 1.0f},
		.attenuation = 1.0f,
		.ambientCoefficient = .2f,
		.coneAngle = 45
	};
	lightSource.camera.projection = perspective(90, 1, 0.1, 100);

	return (struct StreetLight) {
		.lamp = lightSource,
		.model = modelLightPost,
		.position = position,
	};
}
