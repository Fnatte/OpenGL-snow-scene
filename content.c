#include "content.h"

GLfloat ground[] = {
	-35,2,-35,
	-35,2, 15,
	15,2, 15,
	15,2,-35
};
GLuint groundIndices[] = {0, 1, 2, 0, 2, 3};

void loadContent() {
	modelGround = LoadDataToModel(ground,	NULL,	NULL,	NULL,	groundIndices, 4,	6);
	modelCube = LoadModelPlus("./models/octagon.obj");
	modelPlane = LoadModelPlus("./models/plane.obj");

	LoadTGATextureSimple("./textures/ground/diffuse.tga", &textureGroundDiffuse);
	LoadTGATextureSimple("./textures/ground/specular.tga", &textureGroundSpecular);
	LoadTGATextureSimple("./textures/ground/normal.tga", &textureGroundNormal); 
}
