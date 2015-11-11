#ifndef MAIN_H
#define MAIN_H


GLuint fullProgram, plainProgram, instancingProgram, skyboxProgram;

void reshapeViewport(GLsizei w, GLsizei h);
void initUserCamera();
void initpointLight();
void rotateLight();
mat4 getShadowMapTransform(mat4 modelViewProjectionTransform);
void renderScene(void);
void handleMouse(int x, int y);
void onTimer(int value);
int main(int argc, char** argv);


#endif
