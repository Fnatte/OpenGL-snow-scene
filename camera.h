#include "./libraries/VectorUtils3.h"


struct Camera {
	vec3 position;
	vec3 normal;
	vec3 target;
	mat4 projection;
};

struct Camera updateCamera(struct Camera c);

struct Camera updateCameraByMouse(struct Camera c, int x, int y);


/**
 * Move the camera around based on keyboard input.
 */
struct Camera moveCameraOnKeyboard(struct Camera c);


/**
 * Introduce a camera shake with the specified magnitude.
 */
struct Camera shakeCamera(struct Camera in, float magnitude);

struct Camera rotateCameraByMouse(struct Camera c, int x, int y);

mat4 getProjectionViewMatrix(struct Camera c);

vec3 cameraDirection(struct Camera c);

struct Camera createUserCamera(vec3 position, vec3 normal, vec3 target, float viewAngle);

const char* getCurrentModeString();
