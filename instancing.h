#ifndef INSTANCING_H
#define INSTANCING_H

#include "light.h"

void initializeInstancingShader(int _count);
void drawModelInstanced(Model *m, mat4 camera, mat4 model, struct Light* light);

#endif
