#include "./libraries/GLUtilities.h"
#include "./libraries/VectorUtils3.h"


mat4 projectionMatrix;
mat4 lookMatrix;
vec3 cameraPos;
vec3 cameraTarget;
vec3 cameraNormal;
vec3 cameraDirection;


void initCamera() {
	cameraPos = (vec3){1.5f, 20.0f, -50.0f};
	cameraTarget = (vec3){10.0f, 15.0f, 5.0f};
	cameraNormal = (vec3){0.0f, 1.0f, 0.0f};
	lookMatrix = lookAtv(cameraPos, cameraTarget, cameraNormal);
}


vec3 moveCameraOnKeyboard(vec3 in, vec3 cameraNormal, vec3 cameraDirection) {
	vec3 forward;
	vec3 leftV;
	if(keyIsDown('x')){
		forward = ScalarMult(cameraDirection, 10.0f);
		leftV = ScalarMult(CrossProduct(cameraDirection, cameraNormal), 10.0f);
	}
	else{
		forward = ScalarMult(cameraDirection, 1.0f);
		leftV = ScalarMult(CrossProduct(cameraDirection, cameraNormal), 1.0f);
	}

	if(keyIsDown('w')) {
		in.x += forward.x;
		in.y += forward.y;
		in.z += forward.z;
	}
	else if (keyIsDown('s')) {
		in.x -= forward.x;
		in.y -= forward.y;
		in.z -= forward.z;
	}

	if(keyIsDown('a')){
		in.x -= leftV.x;
		in.y -= leftV.y;
		in.z -= leftV.z;
	}
	else if(keyIsDown('d')){
		in.x += leftV.x;
		in.y += leftV.y;
		in.z += leftV.z;
	}

	if(keyIsDown('q'))
		in.y += 0.1;
	else if(keyIsDown('e'))
		in.y -= 0.1;

	printError("moveonkeyinputrelativecamera()");
	return in;
}


vec3 smoothRandomMovement(vec3 in, float magnitude) {
	GLfloat time = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;

}


void handleMouse(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	cameraTarget = (vec3)
		{cos((float)x / width * M_PI * 2) * sin((float)y / height * M_PI),
		 -(float)y / height + 0.5,
		 sin((float)x / width * M_PI * 2) * sin((float)y / height * M_PI)};
	cameraTarget = VectorAdd(cameraTarget, cameraPos);

	lookMatrix = lookAtv(cameraPos, cameraTarget, cameraNormal);
	cameraDirection = Normalize(VectorSub(cameraTarget, cameraPos));
	printError("handleMouse()");
}


void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	projectionMatrix = perspective(90, (GLfloat)w/(GLfloat)h, 0.1, 1000);
}


mat4 getProjectionViewMatrix() {
	cameraPos = moveCameraOnKeyboard(cameraPos, cameraNormal, cameraDirection);
	cameraTarget = moveCameraOnKeyboard(cameraTarget, cameraNormal, cameraDirection);

	lookMatrix = lookAtv(cameraPos, cameraTarget, cameraNormal);
	return Mult(projectionMatrix, lookMatrix);
}

vec3 getCameraPos() {
	return cameraPos;
}
