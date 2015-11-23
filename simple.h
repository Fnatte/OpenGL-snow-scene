#include <GL/glew.h>

#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"
#include "light.h"

GLuint simpleProgram;

void initializeSimpleShader();
void drawSimple(Model *m, mat4 cameraTransform, mat4 modelTransform, GLuint texture);
