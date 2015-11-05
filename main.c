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


struct Camera userCamera;

#define RENDER_WIDTH 640.0
#define RENDER_HEIGHT 480.0

// We assign one texture unit in which to store the transformation.
#define TEX_UNIT 0

#define NEAR 1.0
#define FAR 100.0
#define W 600
#define H 600

//Camera position
Point3D p_camera = {32,20,0};

//Camera lookAt
Point3D l_camera = {2,0,-10};

//Light mouvement circle radius
float light_mvnt = 40.0f; // At 30 we get edge artifacts

//Light position
Point3D p_light = {40,20,0};

//Light lookAt
//Point3D l_light = {0,3,-5};
Point3D l_light = {0,3,-10};

// Use to activate/disable projTexShader
GLuint fullProgram, plainProgram, instancingProgram;
GLuint projTexMapUniform;

FBOstruct *fbo;

//-----------------------------matrices------------------------------
mat4	modelViewMatrix, textureMatrix;
mat4 projectionMatrix;

mat4 transCubes;

// Arrays for ground and the Model references
GLfloat groundcolor[] = {0.3f,0.3f,0.3f,1};
GLfloat ground[] = {
	-35,2,-35,
	-35,2, 15,
	15,2, 15,
	15,2,-35
};
GLuint groundIndices[] = {0, 1, 2, 0, 2, 3};
Model *groundModel, *torusModel, *sphereModel;


void initUserCamera() {
	vec3 position = (vec3){1.5f, 20.0f, -50.0f};
	vec3 normal = (vec3){0.0f, 1.0f, 0.0f};
	vec3 target = (vec3){10.0f, 15.0f, 5.0f};
	userCamera = createUserCamera(position, normal, target, 90.0);
}

void loadShadowShader() {
	fullProgram = loadShaders("shaders/full.vert", "shaders/full.frag");
	projTexMapUniform = glGetUniformLocation(fullProgram,"textureUnit");
	plainProgram = loadShaders("shaders/plain.vert", "shaders/plain.frag");
	instancingProgram = loadShaders("./shaders/instancing.vert", "./shaders/instancing.frag");
}


// This update only change the position of the light.
void updatePositions(void) {
	p_light.x = light_mvnt * cos(glutGet(GLUT_ELAPSED_TIME)/1000.0);
	p_light.z = light_mvnt * sin(glutGet(GLUT_ELAPSED_TIME)/1000.0);
}


// Build the transformation sequence for the light source path,
// by copying from the ordinary camera matrices.
void setTextureMatrix(void)
{
	mat4 scaleBiasMatrix;

	IdentityMatrix(textureMatrix);

	// Scale and bias transform, moving from unit cube [-1,1] to [0,1]
	scaleBiasMatrix = Mult(T(0.5, 0.5, 0.0), S(0.5, 0.5, 1.0));
	textureMatrix = Mult(Mult(scaleBiasMatrix, projectionMatrix), modelViewMatrix);
	// Multiply modelview and transformation matrices
}


void loadObjects(void) {
	// The shader must be loaded before this is called!
	if (fullProgram == 0)
		printf("Warning! Is the shader not loaded?\n");
	groundModel = LoadDataToModel(ground,	NULL,	NULL,	NULL,	groundIndices, 4,	6);
	torusModel = LoadModelPlus("models/torus.obj");
	sphereModel = LoadModelPlus("models/sphere.obj");
	transCubes = T(-10, 100, -10);
}

void drawObjects() {
  mat4 mv2, tx2, trans;

	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Ground
	glUniform1f(glGetUniformLocation(fullProgram, "shade"), 0.3); // Dark ground
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "modelViewMatrix"), 1, GL_TRUE, modelViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "textureMatrix"), 1, GL_TRUE, textureMatrix.m);
	DrawModel(groundModel, fullProgram, "in_Position", NULL, NULL);

	glUniform1f(glGetUniformLocation(fullProgram, "shade"), 0.9); // Brighter objects

	// One torus
	trans = Mult(T(0,4,-16), S(2.0, 2.0, 2.0)); // Apply on both
	mv2 = Mult(modelViewMatrix, trans); // Apply on both
	tx2 = Mult(textureMatrix, trans);
	// Upload both!
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "modelViewMatrix"), 1, GL_TRUE, mv2.m);
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "textureMatrix"), 1, GL_TRUE, tx2.m);
	DrawModel(torusModel, fullProgram, "in_Position", NULL, NULL);

	// The other torus
	trans = Mult(Mult(T(0,4,-16), Ry(3.14/2)), S(2.0, 2.0, 2.0)); // Apply on both
	mv2 = Mult(modelViewMatrix, trans);
	tx2 = Mult(textureMatrix, trans);
	// Upload both!
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "modelViewMatrix"), 1, GL_TRUE, mv2.m);
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "textureMatrix"), 1, GL_TRUE, tx2.m);
	DrawModel(torusModel, fullProgram, "in_Position", NULL, NULL);

	// The sphere
	trans = Mult(T(0,4,-5), S(5.0, 5.0, 5.0));
	trans = Mult(T(0,4,-4), S(5.0, 5.0, 5.0));
	mv2 = Mult(modelViewMatrix, trans); // Apply on both
	tx2 = Mult(textureMatrix, trans);
	// Upload both!
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "modelViewMatrix"), 1, GL_TRUE, mv2.m);
	glUniformMatrix4fv(glGetUniformLocation(fullProgram, "textureMatrix"), 1, GL_TRUE, tx2.m);
	DrawModel(sphereModel, fullProgram, "in_Position", NULL, NULL);
}

void renderScene(void) {
	userCamera = moveCameraOnKeyboard(userCamera);
	mat4 projectionViewMatrix = getProjectionViewMatrix(userCamera);
	// Change light positions
	updatePositions();

	// Setup projection matrix
	projectionMatrix = perspective(45, RENDER_WIDTH/RENDER_HEIGHT, 10, 4000);

	// Setup the modelview from the light source
	modelViewMatrix = lookAt(p_light.x, p_light.y, p_light.z,
													 l_light.x, l_light.y, l_light.z, 0,1,0);
	// Using the result from lookAt, add a bias to position the result properly in texture space
	setTextureMatrix();

	// 1. Render scene to FBO
	useFBO(fbo, NULL, NULL);
	glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Depth only
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the simple shader
	glUseProgram(fullProgram);
	glUniform1i(projTexMapUniform,TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,0);

	drawObjects();

	mat4 trans = Mult(T(0,4,-16), S(2.0, 2.0, 2.0)); // Apply on both
	projectionViewMatrix = Mult(projectionMatrix, trans);
	drawModelInstanced(sphereModel, instancingProgram, transCubes, projectionViewMatrix);
	glFlush();

	//2. Render from camera.
	// Now rendering from the camera POV

	useFBO(NULL, fbo, NULL);

	glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);

	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the projTex shader
	glUseProgram(fullProgram);
	glUniform1i(projTexMapUniform,TEX_UNIT);
	glActiveTexture(GL_TEXTURE0 + TEX_UNIT);
	glBindTexture(GL_TEXTURE_2D,fbo->depth);


	// Setup the modelview from the camera
	modelViewMatrix = lookAtv(userCamera.position, userCamera.target, userCamera.normal);

	glCullFace(GL_BACK);
	drawObjects();

	trans = Mult(T(0,4,-16), S(2.0, 2.0, 2.0)); // Apply on both
	projectionViewMatrix = Mult(projectionMatrix, trans);
	drawModelInstanced(sphereModel, instancingProgram, transCubes, projectionViewMatrix);

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
	glutInitWindowPosition(100,100);
	glutInitWindowSize(RENDER_WIDTH,RENDER_HEIGHT);
	glutInitContextVersion(3, 1);
	glutCreateWindow("Shadow mapping demo");
	glutPassiveMotionFunc(handleMouse);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	// https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW_.28OpenGL_Extension_Wrangler.29
	printError ("It should be safe to ignore this, see comment in code.");
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	dumpInfo();

	loadShadowShader();
	loadObjects();
	initUserCamera();
	initKeymapManager();
	setupInstancedVertexAttributes(instancingProgram, 10);
	fbo = initFBO2(RENDER_WIDTH,RENDER_HEIGHT, 0, 1);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, &onTimer, 0);

	glutMainLoop();
}
