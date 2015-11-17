#include "./libraries/VectorUtils3.h"
#include "./libraries/LoadObject.h"

void drawModelWithLocations(Model *m, GLuint positionLocation, GLuint normalLocation, GLuint textureLocation);
float* getRandFloatArray(int size, float upperLimit, float lowerLimit);
vec3 getRandVec3(float upperLimit, float lowerLimit);

/**
 * Prints a vec3 to stdout.
 */
void printVec3(vec3 in);
