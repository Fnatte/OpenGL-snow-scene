#include <stdio.h>
#include <math.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/VectorUtils3.h"
#include "camera.h"

#define MODE_MOVIE 0
#define MODE_INTERACTIVE 1

int currentMode = MODE_MOVIE;

int wasModeKeyDown = 0;

struct Camera updateCamera(struct Camera c) {
	struct Camera nextCamera;

	// Allow mode change by keyboard
	if(wasModeKeyDown && !keyIsDown(' ')) {
		currentMode = (currentMode + 1) % 2;
		printf("Mode changed to %s\n", getCurrentModeString());
	}
	wasModeKeyDown = keyIsDown(' ');

	// Calculate next camera state
	switch(currentMode) {
		case MODE_MOVIE:
			nextCamera = shakeCamera(c, 1);
			break;
		case MODE_INTERACTIVE:
			nextCamera = moveCameraOnKeyboard(c);
			break;
		default:
			nextCamera = c;
			break;
	}


	return nextCamera;
}

struct Camera updateCameraByMouse(struct Camera c, int x, int y) {
	if(currentMode == MODE_INTERACTIVE) {
		return rotateCameraByMouse(c, x, y);
	}

	return c;
}

struct Camera moveCameraOnKeyboard(struct Camera c) {
	vec3 forward;
	vec3 leftV;
	if(keyIsDown('x')){
		forward = ScalarMult(cameraDirection(c), 10.0f);
		leftV = ScalarMult(CrossProduct(cameraDirection(c), c.normal), 10.0f);
	}
	else{
		forward = ScalarMult(cameraDirection(c), 1.0f);
		leftV = ScalarMult(CrossProduct(cameraDirection(c), c.normal), 1.0f);
	}

	if(keyIsDown('w')) {
		c.position = VectorAdd(c.position, forward);
		c.target = VectorAdd(c.target, forward);
	}
	else if (keyIsDown('s')) {
		c.position = VectorSub(c.position, forward);
		c.target = VectorSub(c.target, forward);
	}

	if(keyIsDown('a')){
		c.position = VectorSub(c.position, leftV);
		c.target = VectorSub(c.target, leftV);
	}
	else if(keyIsDown('d')){
		c.position = VectorAdd(c.position, leftV);
		c.target = VectorAdd(c.target, leftV);
	}

	if(keyIsDown('q')) {
		c.position.y += 0.1;
		c.target.y += 0.1;
	}
	else if(keyIsDown('e')) {
		c.position.y -= 0.1;
		c.target.y -= 0.1;
	}
	printError("moveonkeyinputrelativecamera()");
	return c;
}


struct Camera shakeCamera(struct Camera c, float magnitude) {
	GLfloat time = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
	c.target.y += sin(time) * magnitude;
	return c;
}


struct Camera rotateCameraByMouse(struct Camera c, int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	c.target = (vec3)
		{cos((float)x / width * M_PI * 2) * sin((float)y / height * M_PI),
		 -(float)y / height + 0.5,
		 sin((float)x / width * M_PI * 2) * sin((float)y / height * M_PI)};
	c.target = VectorAdd(c.target, c.position);
	return c;
}


mat4 getProjectionViewMatrix(struct Camera c) {
	mat4 lookMatrix = lookAtv(c.position, c.target, c.normal);
	return Mult(c.projection, lookMatrix);
}


vec3 cameraDirection(struct Camera c) {
	return Normalize(VectorSub(c.target, c.position));
}


struct Camera createUserCamera(vec3 position, vec3 normal, vec3 target, float viewAngle) {
	struct Camera c = (struct Camera)
		{ position, normal, target,
			perspective(viewAngle, (GLfloat)glutGet(GLUT_WINDOW_X)/(GLfloat)glutGet(GLUT_WINDOW_Y), 10, 4000) };
	return c;
}

const char* getCurrentModeString() {
	static char* strings[] = { "movie", "interactive" };
	static char* badModeString = "invalid";

	if(currentMode < 0 || currentMode >= 2) {
		return badModeString;
	}

	return strings[currentMode];
}
