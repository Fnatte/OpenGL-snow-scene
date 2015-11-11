#include "./libraries/VectorUtils3.h"

struct Camera {
	vec3 position;
	vec3 normal;
	vec3 target;
	mat4 projection;
};

struct ShakeableCamera {
	struct Camera base;
	struct Camera original;
	float shakeElapsedTime;
};

/**
 * Get string that represents the current camera mode
 */
const char* getCurrentModeString();

/**
 * Creates a new Camera struct.
 */
struct Camera createCamera(vec3 position, vec3 normal, vec3 target);

/*
 * Updates the specified camera with mouse movement based on the current mode.
 */
void updateCameraByMouse(struct Camera *c, int x, int y);

/**
 * Move the camera around based on keyboard input.
 */
void moveCameraOnKeyboard(struct Camera *c);

/*
 * Changes the camera target with the rotation specified by 
 * the given mouse coordinates.
 */
void rotateCameraByMouse(struct Camera *c, int x, int y);

/*
 * Returns the the projection view matrix.
 */
mat4 getProjectionViewMatrix(struct Camera *c);

/*
 * Returns a vec3 that indicates the direction the camera is looking.
 */
vec3 cameraDirection(struct Camera *c);

/*
 * Set the projection matrix of the camera as a perspective matrix. 
 */
void setCameraPerspective(struct Camera *camera, float viewAngle, int width, int height, float near, float far);

/*
 * Creates a new ShakableCamera struct.
 */
struct ShakeableCamera createShakeableCamera(vec3 position, vec3 normal, vec3 target);

/**
 * Introduce a camera shake with the specified magnitude.
 */
void shakeCamera(struct ShakeableCamera *camera);

/**
 * Updates the specified camera based on the current mode.
 */
void updateCamera(struct ShakeableCamera *c);

