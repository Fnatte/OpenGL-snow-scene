#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libraries/LoadTGA.h"
#include "libraries/VectorUtils3.h"
#include "libraries/GLUtilities.h"
#include "libraries/LoadObject.h"

#include "main.h"
#include "instancing.h"
#include "ground.h"
#include "camera.h"
#include "content.h"
#include "skybox.h"
#include "full.h"
#include "plain.h"


#define FBO_RES 2048


struct Camera pointLight;
FBOstruct *fbo;
mat4 transCubes;
GLuint TEX_UNIT = 0;
mat4 transLightPost;


void reshapeViewport(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	userCamera.base.projection = perspective(90, w/h, 0.1, 1000);
}


void initUserCamera() {
	vec3 position = (vec3){1.5f, 20.0f, -50.0f};
	vec3 normal = (vec3){0.0f, 1.0f, 0.0f};
	vec3 target = (vec3){10.0f, 15.0f, 5.0f};
	userCamera = createShakeableCamera(position, normal, target);
	userCamera.base.projection = perspective(90, (GLfloat)glutGet(GLUT_WINDOW_X) / (GLfloat)glutGet(GLUT_WINDOW_Y), 0.1, 1000);
}


void initPointLight() {
	vec3 position = (vec3){40, 20, 0};
	vec3 target = (vec3){0, 3, -10};
	vec3 normal = CrossProduct(position, target);
	pointLight = createCamera(position, normal, target);
	pointLight.projection = perspective(90, 1, 10, 4000);
}


void initShaders() {
	initializeFullShader();
	initializeInstancingShader(10);
	initializeSkyboxShader();
	plainProgram = loadShaders("shaders/plain.vert", "shaders/plain.frag");

	glUseProgram(plainProgram);
	glUniform1i(glGetUniformLocation(plainProgram, "textureUnit"), TEX_UNIT);

	glUseProgram(fullProgram);
	glUniform1i(glGetUniformLocation(fullProgram, "textureUnit"), TEX_UNIT);
}


void rotateLight(void) {
	pointLight.position.x = 30.0 * -cos(glutGet(GLUT_ELAPSED_TIME)/10000.0);
	pointLight.position.z = 30.0 * -sin(glutGet(GLUT_ELAPSED_TIME)/10000.0);
}


mat4 getShadowMapTransform(mat4 modelViewProjectionTransform) {
	// Scale and bias transform, moving from unit cube [-1,1] to [0,1]
	mat4 scaleBiasMatrix = Mult(T(0.5, 0.5, 0.0), S(0.5, 0.5, 1.0));
	return Mult(scaleBiasMatrix, modelViewProjectionTransform);
}


void renderScene(void) {
	rotateLight();
	updateCamera(&userCamera);

	mat4 lightTransform = getProjectionViewMatrix(&pointLight);
	mat4 cameraTransform = getProjectionViewMatrix((struct Camera *)&userCamera);
	mat4 shadowMapTransform = getShadowMapTransform(lightTransform);

	glUseProgram(plainProgram);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 1. Render scene to FBO
	useFBO(fbo, NULL, NULL);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Depth only
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Using the simple shader
	drawObjects(plainProgram, lightTransform, shadowMapTransform);
	drawModelInstanced(modelCube, transCubes, lightTransform);
	glFlush();
	printError("Draw me like one of your french girls");

	// 2. Render from camera.
	useFBO(NULL, fbo, NULL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSkybox(cameraTransform);

	glUseProgram(fullProgram);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,fbo->depth);

	drawFull(modelLightPost, cameraTransform, shadowMapTransform, transLightPost);

	drawModelInstanced(modelCube, transCubes, cameraTransform);
	drawGroundWithProgram(fullProgram, cameraTransform, shadowMapTransform);
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
	glutCreateWindow("Shadow mapping demo");
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
	initializeGround(modelPlane, fullProgram);

	transCubes = T(-20, 100, -20);
	transLightPost =  S(5.0, 5.0, 5.0);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, &onTimer, 0);

	glutMainLoop();
}
