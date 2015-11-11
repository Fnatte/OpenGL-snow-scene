#ifndef CAMERA_H
#define CAMERA_H

#include "./libraries/VectorUtils3.h"


struct Camera {
	vec3 position;
	vec3 normal;
	vec3 target;
	mat4 projection;
};


/**
 * Move the camera around based on keyboard input.
 */
struct Camera moveCameraOnKeyboard(struct Camera c);


/**
 * Introduce a camera shake with the specified magnitude.
 */
struct Camera smoothRandomMovement(struct Camera in, float magnitude);

struct Camera rotateCameraByMouse(struct Camera c, int x, int y);

mat4 getProjectionViewMatrix(struct Camera c);

vec3 cameraDirection(struct Camera c);

struct Camera createUserCamera(vec3 position, vec3 normal, vec3 target, float viewAngle);

#endif
