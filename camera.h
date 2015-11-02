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

void initCamera();

void handleMouse(int x, int y);

void reshape(GLsizei w, GLsizei h);

mat4 getProjectionViewMatrix();

vec3 getCameraPos();

vec3 cameraDirection(struct Camera c);
