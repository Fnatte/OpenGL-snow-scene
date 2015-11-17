#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libraries/VectorUtils3.h"
#include "libraries/GLUtilities.h"
#include "libraries/LoadObject.h"

#include "main.h"
#include "instancing.h"
#include "content.h"
#include "skybox.h"
#include "full.h"
#include "plain.h"

#define FBO_RES 2048

struct Light pointLight;
FBOstruct *fbo;

mat4 cubesTransform;
mat4 lightPostTransform;

void reshapeViewport(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	userCamera.base.projection = perspective(90, 1.0 * w / h, 0.1, 1000);
}


void initUserCamera() {
	vec3 position = (vec3){40, 20, 0};
	vec3 target = (vec3){0, 3, -10};
	vec3 normal = (vec3){0, 1, 0};
	userCamera = createShakeableCamera(position, normal, target);
	userCamera.base.projection = perspective(90, (GLfloat)glutGet(GLUT_WINDOW_X) / (GLfloat)glutGet(GLUT_WINDOW_Y), 0.1, 1000);
}


void initPointLight() {
	vec3 position = (vec3){0.199828, 16.732456, -1.134634};
	vec3 target = (vec3){0.287315, 16.304140, -1.340088};
	vec3 normal = CrossProduct(position, target);

	pointLight = (struct Light) {
		.camera = createCamera(position, normal, target),
		.intensities = (vec3){1.0f, 1.0f, 1.0f},
		.attenuation = 1.0f,
		.ambientCoefficient = 1.0f,
		.coneAngle = 45
	};

	pointLight.camera.projection = perspective(90, 1, 10, 4000);

	setLight(&pointLight);
}


void initShaders() {
	initializeFullShader();
	initializePlainShader();
	initializeInstancingShader(10);
	initializeSkyboxShader();
}


mat4 getShadowMapTransform(mat4 modelViewProjectionTransform) {
	// Scale and bias transform, moving from unit cube [-1,1] to [0,1]
	mat4 scaleBiasMatrix = Mult(T(0.5, 0.5, 0.0), S(0.5, 0.5, 1.0));
	return Mult(scaleBiasMatrix, modelViewProjectionTransform);
}


void renderScene(void) {
	updateCamera(&userCamera);

	mat4 lightTransform = getProjectionViewMatrix((struct Camera *) &pointLight);
	mat4 cameraTransform = getProjectionViewMatrix((struct Camera *) &userCamera);
	mat4 shadowMapTransform = getShadowMapTransform(lightTransform);

	// 1. Render scene to FBO
	useFBO(fbo, NULL, NULL);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawPlain(modelLightPost, lightTransform, lightPostTransform);
	drawModelInstanced(modelCube, cubesTransform, lightTransform);
	printError("Draw me like one of your french girls");

	// 2. Render from camera.
	useFBO(NULL, fbo, NULL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSkybox(cameraTransform);
	drawFull(modelLightPost, cameraTransform, lightPostTransform, shadowMapTransform, textureMetal, fbo->depth);
	drawModelInstanced(modelCube, cubesTransform, cameraTransform);
	drawFull(modelPlane, cameraTransform, T(0,0,0), shadowMapTransform, textureGroundDiffuse, fbo->depth);

	printError("Draw me like one of your italian girls");
	glutSwapBuffers();
}


void handleMouse(int x, int y) {
	updateCameraByMouse((struct Camera *) &userCamera, x, y);
}


void onTimer(int value) {
	glutPostRedisplay();
	glutTimerFunc(16, &onTimer, value);
	printError("OnTimer()");
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 1);
	glutCreateWindow("Let it Snow!");
	glutPassiveMotionFunc(handleMouse);
	glutReshapeFunc(reshapeViewport);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	// https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW_.28OpenGL_Extension_Wrangler.29
	printError ("It should be safe to ignore this, see comment in code.");
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	dumpInfo();

	fbo = initFBO2(FBO_RES, FBO_RES, 0, 1);
	initShaders();
	loadContent();
	initUserCamera();
	initPointLight();
	initKeymapManager();

	cubesTransform = T(-20, 100, -20);
	lightPostTransform =  S(1.5, 1.5, 1.5);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, &onTimer, 0);

	glutMainLoop();
}
