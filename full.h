#include <GL/glew.h>

#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"
#include "light.h"

#define MAX_LIGHTS 10


GLuint fullProgram;


void initializeFullShader();
void drawFull(Model *m, mat4 cameraTransform, mat4 modelTransform, mat4 shadowMapTransform, GLuint texture,
              GLuint shadowMap, struct StreetLight* light, int nrLights, vec3 cameraPosition);
