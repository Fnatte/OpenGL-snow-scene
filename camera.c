#include "./libraries/GLUtilities.h"
#include "./libraries/VectorUtils3.h"

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
