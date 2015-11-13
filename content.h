#include <GL/glew.h>
#include <GL/glut.h>

#include "libraries/LoadObject.h"
#include "libraries/LoadTGA.h"

Model *modelGround;
Model *modelCube;
Model *modelPlane;
Model *modelSkybox;
Model *modelLightPost;

GLuint textureGroundDiffuse;
GLuint textureGroundSpecular;
GLuint textureGroundNormal;
GLuint textureSkybox;

void loadContent();
