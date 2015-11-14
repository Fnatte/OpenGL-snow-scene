#include <GL/glew.h>

#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"

GLuint fullProgram;

void initializeFullShader(GLuint texUnit);
void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4 shadowMapTransform);
