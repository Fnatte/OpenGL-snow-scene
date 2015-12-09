#include "libraries/LoadObject.h"

#include "light.h"
#include "content.h"


struct ShaderLight getShaderLight(struct Light *light) {
	return (struct ShaderLight) {
		.position = light->camera.position,
		.intensities = light->intensities,
		.coneDirection = VectorSub(light->camera.target, light->camera.position),
		.coneAngle = light->coneAngle,
		.ambientCoefficient = light->ambientCoefficient
	};
}


struct StreetLight createStreetLight(vec3 position) {
	vec3 lightPosition = (vec3) {
		position.x, position.y + 18.5f, position.z - 1.67790f };
	vec3 lightTarget = (vec3) {
		position.x + 0.287315f, position.y, position.z - 8.340088f };
	vec3 lightNormal = CrossProduct(lightPosition, lightTarget);

	struct Light lightSource = (struct Light) {
		.camera = createCamera(lightPosition, lightNormal, lightTarget),
		.intensities = (vec3){1.0f, 1.0f, 1.0f},
		.ambientCoefficient = 0.03f,
		.coneAngle = 100
	};
	lightSource.camera.projection = perspective(lightSource.coneAngle, 1, 0.05, 100);

	return (struct StreetLight) {
		.lamp = lightSource,
		.model = modelLightPost,
		.position = position,
	};
}
