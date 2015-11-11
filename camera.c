#include <stdio.h>
#include <math.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/VectorUtils3.h"
#include "camera.h"

#define MODE_MOVIE 0
#define MODE_INTERACTIVE 1

int currentMode = MODE_MOVIE;

int wasModeKeyDown = 0;

struct Camera createCamera(vec3 position, vec3 normal, vec3 target) {
	struct Camera c = (struct Camera) { position, normal, target, IdentityMatrix() };
	return c;
}

struct ShakeableCamera createShakeableCamera(vec3 position, vec3 normal, vec3 target) {
	return (struct ShakeableCamera) {
		createCamera(position, normal, target),
		createCamera(position, normal, target),
		0
	};
}

void updateCamera(struct ShakeableCamera *c) {
	// Allow mode change by keyboard
	if(wasModeKeyDown && !keyIsDown(' ')) {
		currentMode = (currentMode + 1) % 2;
		printf("Mode changed to %s\n", getCurrentModeString());

		// When we switch to movie mode we copy the base camera to the original
		// camera, which will hold the camera state that we shake around. We must
		// also reset some of the shake state variables.
		if(currentMode == MODE_MOVIE) {
			c->original.target = c->base.target;
			c->original.position = c->base.position;
			c->shakeElapsedTime = 0;
		}

	}
	wasModeKeyDown = keyIsDown(' ');


	// Modify camera state
	c->shakeElapsedTime += 1;
	switch(currentMode) {
		case MODE_MOVIE:
			shakeCamera(c);
			break;
		case MODE_INTERACTIVE:
			moveCameraOnKeyboard((struct Camera *) c);
			break;
	}
}

void updateCameraByMouse(struct Camera *c, int x, int y) {
	if(currentMode == MODE_INTERACTIVE) {
		rotateCameraByMouse(c, x, y);
	}
}

void moveCameraOnKeyboard(struct Camera *c) {
	vec3 forward;
	vec3 leftV;
	vec3 dir = cameraDirection(c);
	if(keyIsDown('x')){
		forward = ScalarMult(dir, 10.0f);
		leftV = ScalarMult(CrossProduct(dir, c->normal), 10.0f);
	}
	else{
		forward = ScalarMult(cameraDirection(c), 1.0f);
		leftV = ScalarMult(CrossProduct(dir, c->normal), 1.0f);
	}

	if(keyIsDown('w')) {
		c->position = VectorAdd(c->position, forward);
		c->target = VectorAdd(c->target, forward);
	}
	else if (keyIsDown('s')) {
		c->position = VectorSub(c->position, forward);
		c->target = VectorSub(c->target, forward);
	}

	if(keyIsDown('a')){
		c->position = VectorSub(c->position, leftV);
		c->target = VectorSub(c->target, leftV);
	}
	else if(keyIsDown('d')){
		c->position = VectorAdd(c->position, leftV);
		c->target = VectorAdd(c->target, leftV);
	}

	if(keyIsDown('q')) {
		c->position.y += 0.1;
		c->target.y += 0.1;
	}
	else if(keyIsDown('e')) {
		c->position.y -= 0.1;
		c->target.y -= 0.1;
	}

	printError("moveonkeyinputrelativecamera()");
}

void shakeCamera(struct ShakeableCamera *c) {
	vec3 offset = SetVector(
		sin(c->shakeElapsedTime * 0.25) * 0.5,
		0, 
		cos(c->shakeElapsedTime * 0.35) * 0.15
	);

	c->base.target = VectorAdd(c->original.target, offset);
}

void rotateCameraByMouse(struct Camera *c, int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	c->target = (vec3)
		{cos((float)x / width * M_PI * 2) * sin((float)y / height * M_PI),
		 -(float)y / height + 0.5,
		 sin((float)x / width * M_PI * 2) * sin((float)y / height * M_PI)};
	c->target = VectorAdd(c->target, c->position);
}

mat4 getProjectionViewMatrix(struct Camera *c) {
	mat4 lookMatrix = lookAtv(c->position, c->target, c->normal);
	return Mult(c->projection, lookMatrix);
}

vec3 cameraDirection(struct Camera *c) {
	return Normalize(VectorSub(c->target, c->position));
}



const char* getCurrentModeString() {
	static char* strings[] = { "movie", "interactive" };
	static char* badModeString = "invalid";

	if(currentMode < 0 || currentMode >= 2) {
		return badModeString;
	}

	return strings[currentMode];
}
