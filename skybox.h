#ifndef SKYBOX_H
#define SKYBOX_H


#include "libraries/LoadObject.h"
#include "libraries/VectorUtils3.h"

#include "camera.h"


void initializeSkyboxShader();
void drawSkybox(mat4 transform);


#endif
