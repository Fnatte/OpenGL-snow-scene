#include <math.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/VectorUtils3.h"
#include "camera.h"


mat4 lookMatrix;
struct Camera userCamera;

void initCamera() {
	userCamera.position = (vec3){1.5f, 20.0f, -50.0f};
	userCamera.target = (vec3){10.0f, 15.0f, 5.0f};
	userCamera.normal = (vec3){0.0f, 1.0f, 0.0f};
	lookMatrix = lookAtv(userCamera.position, userCamera.target, userCamera.normal);
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
		c.position.x += forward.x;
		c.target.x += forward.x;
		c.position.y += forward.y;
		c.target.y += forward.y;
		c.position.z += forward.z;
		c.target.z += forward.z;
	}
	else if (keyIsDown('s')) {
		c.position.x -= forward.x;
		c.target.x -= forward.x;
		c.position.y -= forward.y;
		c.target.y -= forward.y;
		c.position.z -= forward.z;
		c.target.z -= forward.z;
	}

	if(keyIsDown('a')){
		c.position.x -= leftV.x;
		c.target.x -= leftV.x;
		c.position.y -= leftV.y;
		c.target.y -= leftV.y;
		c.position.z -= leftV.z;
		c.target.z -= leftV.z;
	}
	else if(keyIsDown('d')){
		c.position.x += leftV.x;
		c.target.x += leftV.x;
		c.position.y += leftV.y;
		c.target.y += leftV.y;
		c.position.z += leftV.z;
		c.target.z += leftV.z;
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


struct Camera smoothRandomMovement(struct Camera c, float magnitude) {
	GLfloat time = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
	c.target.y += sin(time) * magnitude;
	return c;
}


void handleMouse(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	userCamera.target = (vec3)
		{cos((float)x / width * M_PI * 2) * sin((float)y / height * M_PI),
		 -(float)y / height + 0.5,
		 sin((float)x / width * M_PI * 2) * sin((float)y / height * M_PI)};
	userCamera.target = VectorAdd(userCamera.target, userCamera.position);

	lookMatrix = lookAtv(userCamera.position, userCamera.target, userCamera.normal);
	printError("handleMouse()");
}


void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	userCamera.projection = perspective(90, (GLfloat)w/(GLfloat)h, 0.1, 1000);
}


mat4 getProjectionViewMatrix() {
	userCamera = moveCameraOnKeyboard(userCamera);
	lookMatrix = lookAtv(userCamera.position, userCamera.target, userCamera.normal);
	return Mult(userCamera.projection, lookMatrix);
}

vec3 getCameraPos() {
	return userCamera.position;
}

vec3 cameraDirection(struct Camera c) {
	return Normalize(VectorSub(c.target, c.position));
}
