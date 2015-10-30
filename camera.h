/**
 * Move the camera around based on keyboard input.
 */
vec3 moveCameraOnKeyboard(vec3 in, vec3 cameraNormal, vec3 cameraDirection);


/**
 * Introduce a camera shake with the specified magnitude.
 */
vec3 smoothRandomMovement(vec3 in, float magnitude);
