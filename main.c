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
#include "shadow.h"
#include "content.h"

#define TEX_UNIT 0
#define FBO_RES 2048


struct Camera userCamera;
struct Camera pointLight;

GLuint fullProgram, plainProgram, instancingProgram, skyboxProgram;

FBOstruct *fbo;

mat4 transCubes;

void reshapeViewport(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	userCamera.projection = perspective(90, (GLfloat)w/(GLfloat)h, 0.1, 1000);
}

void initUserCamera() {
	vec3 position = (vec3){1.5f, 20.0f, -50.0f};
	vec3 normal = (vec3){0.0f, 1.0f, 0.0f};
	vec3 target = (vec3){10.0f, 15.0f, 5.0f};
	userCamera = createUserCamera(position, normal, target, 90.0);
}


void initpointLight() {
	vec3 position = (vec3){40, 20, 0};
	vec3 target = (vec3){0, 3, -10};
	vec3 normal = CrossProduct(position, target);
	pointLight = (struct Camera)
		{ position, normal, target, perspective(90.0, 1.0 , 10, 4000) };
}


void initShaders() {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");
	plainProgram = loadShaders("shaders/plain.vert", "shaders/plain.frag");
	instancingProgram = loadShaders("./shaders/instancing.vert", "./shaders/instancing.frag");
	skyboxProgram = loadShaders("./shaders/skybox.vert", "./shaders/skybox.frag");

	glUseProgram(plainProgram);
	glUniform1i(glGetUniformLocation(plainProgram, "textureUnit"), TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(fullProgram);
	glUniform1i(glGetUniformLocation(plainProgram, "textureUnit"), TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D, fbo->depth);
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


void loadObjects(void) {
	transCubes = T(-10, 100, -10);
}

void drawObjects(GLuint program, mat4 modelViewProjectionTransform, mat4 shadowMapTransform) {
  mat4 mv2, tx2, trans;

	drawGroundWithProgram(program, modelViewProjectionTransform, shadowMapTransform);

	// The cube
	trans = Mult(T(0,4,-5), S(5.0, 5.0, 5.0));
	mv2 = Mult(modelViewProjectionTransform, trans); // Apply on both
	tx2 = Mult(shadowMapTransform, trans);
	// Upload both!
	if (program == fullProgram) {
		// Brighter objects
		glUniform1f(glGetUniformLocation(program, "shade"), 0.9);
		glUniformMatrix4fv(glGetUniformLocation(program, "shadowMapTransform"), 1, GL_TRUE, tx2.m);
	}
		glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionTransform"), 1, GL_TRUE, mv2.m);
	DrawModel(modelCube, program, "inPosition", NULL, NULL);
}

void drawSkybox(mat4 modelViewProjectionTransform) {
	mat4 cameraTrans = T(userCamera.position.x, userCamera.position.y, userCamera.position.z);

	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projectionViewMatrix"), 1, GL_TRUE, modelViewProjectionTransform.m);
	glBindTexture(GL_TEXTURE_2D, textureSkybox);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "transform"), 1, GL_TRUE, cameraTrans.m);
	DrawModel(modelSkybox, skyboxProgram, "in_Position", NULL, "in_TexCoord");
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}


void renderScene(void) {
	rotateLight();
	userCamera = moveCameraOnKeyboard(userCamera);
	mat4 lightTransform = getProjectionViewMatrix(pointLight);
	mat4 cameraTransform = getProjectionViewMatrix(userCamera);
	mat4 shadowMapTransform = getShadowMapTransform(lightTransform);

	glUseProgram(plainProgram);
	// 1. Render scene to FBO
	useFBO(fbo, NULL, NULL);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Depth only
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Using the simple shader
	drawObjects(plainProgram, lightTransform, shadowMapTransform);
	drawModelInstanced(modelCube, instancingProgram, transCubes, lightTransform);
	glFlush();
	printError("Draw me like one of your french girls");

	// 2. Render from camera.
	useFBO(NULL, fbo, NULL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSkybox(cameraTransform);

	glUseProgram(fullProgram);

	glCullFace(GL_BACK);
	drawObjects(fullProgram, cameraTransform, shadowMapTransform);
	drawModelInstanced(modelCube, instancingProgram, transCubes, cameraTransform);
	printError("Draw me like one of your italian girls");
	glutSwapBuffers();
}


void handleMouse(int x, int y) {
	userCamera = rotateCameraByMouse(userCamera, x, y);
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
	loadObjects();
	initUserCamera();
	initpointLight();
	initKeymapManager();
	setupInstancedVertexAttributes(instancingProgram, 10);
	initializeGround(modelPlane, fullProgram);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, &onTimer, 0);

	glutMainLoop();
}
