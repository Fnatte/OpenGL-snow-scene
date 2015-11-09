#include <stdio.h>
#include <math.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/VectorUtils3.h"
#include "camera.h"


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
