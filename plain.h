#include <GL/glew.h>

#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"
#include "libraries/GLUtilities.h"

GLuint plainProgram;

void initializePlainShader();
void drawPlain(Model* m, mat4 modelViewProjectionTransform, mat4 modelTransform);
