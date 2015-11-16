#include "content.h"

void loadContent() {
	modelCube = LoadModelPlus("./models/octagon.obj");
	modelPlane = LoadModelPlus("./models/plane.obj");
	modelSkybox = LoadModelPlus("./models/skybox.obj");
	modelLightPost = LoadModelPlus("./models/gatlykta.obj");
	modelSnow = LoadModelPlus("./models/snow.obj");

	LoadTGATextureSimple("./textures/ground/diffuse.tga", &textureGroundDiffuse);
	LoadTGATextureSimple("./textures/ground/specular.tga", &textureGroundSpecular);
	LoadTGATextureSimple("./textures/ground/normal.tga", &textureGroundNormal);
	LoadTGATextureSimple("./textures/SkyBox512.tga", &textureSkybox);
	LoadTGATextureSimple("./textures/metal.tga", &textureMetal);
	LoadTGATextureSimple("./textures/snow.tga", &textureSnow);
}
