#include <GL/glew.h>

#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"
#include "light.h"

GLuint fullProgram;


void initializeFullShader();
void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4 shadowMapTransform, GLuint texture,
			  GLuint shadowMap);
void setLight(struct Light *_light);
