#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "./libraries/GLUtilities.h"
#include "./libraries/LoadObject.h"
#include "./libraries/LoadTGA.h"
#include "./libraries/VectorUtils3.h"

#include "main.h"
#include "instancing.h"
#include "ground.h"
#include "camera.h"
#include "shadow.h"

#define VERBOSE 0


unsigned int vertexArrayObjID;
GLuint program;
GLuint skyboxProgram;
GLuint instancingProgram;
GLuint groundProgram;
GLuint plainTextureProgram;
GLuint depthProgram;

Model *octagon;
Model *skybox;
Model *plane;
Model *square;
mat4 transCubes;
mat4 transGround;

GLuint skyTexture;

GLfloat lastT = 0;

int nrInstances = 20;

struct Camera userCamera;

GLfloat squareData[] = {
	-1,-1,0,
	-1,1, 0,
	1,1, 0,
	1,-1, 0};
GLfloat squareTexCoord[] = {
	0, 0,
	0, 1,
	1, 1,
	1, 0};
GLuint squareIndices[] = {0, 1, 2, 0, 2, 3};


void init(void) {
	dumpInfo();

	userCamera = createUserCamera((vec3){1.5f, 20.0f, -50.0f}, (vec3){0.0f, 1.0f, 0.0f}, (vec3){10.0f, 15.0f, 5.0f}, 90.0);

	octagon = LoadModelPlus("./models/octagon.obj");
	skybox = LoadModelPlus("./models/skybox.obj");
	plane = LoadModelPlus("./models/plane2.obj");
	square = LoadDataToModel(squareData, NULL, squareTexCoord, NULL, squareIndices, 4, 6);

	transGround = T(0, 0, 0);
	transCubes = T(-10, 100, -10);

	LoadTGATextureSimple("./textures/SkyBox512.tga", &skyTexture);

	printError("GL inits");
	glClearColor(0.0, 0.0, 0.0, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	// Load and compile shader
	skyboxProgram = loadShaders("./shaders/skybox.vert", "./shaders/skybox.frag");
	instancingProgram = loadShaders("./shaders/instancing.vert", "./shaders/instancing.frag");
	groundProgram = loadShaders("./shaders/ground.vert", "./shaders/ground.frag");
	plainTextureProgram = loadShaders("./shaders/plainTexture.vert", "./shaders/plainTexture.frag");
	depthProgram = loadShaders("./shaders/depth.vert", "./shaders/depth.frag");

	glUseProgram(skyboxProgram);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxProgram, "texUnit"), 0);

	glUseProgram(instancingProgram);
	glUniform1i(glGetUniformLocation(instancingProgram, "texUnit"), 0);
	setupInstancedVertexAttributes(instancingProgram, nrInstances);

	glUseProgram(groundProgram);
	glUniform1i(glGetUniformLocation(groundProgram, "texUnit"), 0);
	initializeGround(plane, groundProgram);

	initShadowMap();

	printError("init(): End");
}

void OnTimer(int value) {
	glutPostRedisplay();
	glutTimerFunc(16, &OnTimer, value);
	printError("OnTimer()");
}

void display(void) {
	printError("pre display");
	mat4 projectionViewMatrix = getProjectionViewMatrix(userCamera);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;

	mat4 groundTransform = Mult(projectionViewMatrix, transGround);

	beginRenderShadowMap();
	glUseProgram(instancingProgram);
	glUniformMatrix4fv(glGetUniformLocation(instancingProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	drawModelInstanced(octagon, instancingProgram, nrInstances, t, transCubes);
	// glUseProgram(depthProgram);
	// glUniformMatrix4fv(glGetUniformLocation(program, "projectionViewWorldTransform"), 1, GL_TRUE, groundTransform.m);
	// drawGroundWithProgram(depthProgram);
	drawGround(groundTransform);
	endRenderShadowMap();

	// useFBO(0L, 0L, 0L);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	// glUseProgram(skyboxProgram);
	// glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	// glBindTexture(GL_TEXTURE_2D, skyTexture);
	// glDisable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);
	// mat4 cameraTrans = T(userCamera.position.x, userCamera.position.y, userCamera.position.z);
	// glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "transform"), 1, GL_TRUE, cameraTrans.m);
	// DrawModel(skybox, skyboxProgram, "in_Position", NULL, "in_TexCoord");
	// glEnable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);

	// // Draw instances
	// glUseProgram(instancingProgram);
	// glUniformMatrix4fv(glGetUniformLocation(instancingProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	// drawModelInstanced(octagon, instancingProgram, nrInstances, t, transCubes);

	// // Draw ground
	// drawGround(groundTransform);

	// Draw shadow map
	glDisable(GL_CULL_FACE);
	useFBO(0L, getShadowMapFBO(), 0L);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(plainTextureProgram);
	glUniform1i(glGetUniformLocation(plainTextureProgram, "image"), 0);
	DrawModel(square, plainTextureProgram, "in_Position", NULL, "in_TexCoord");
	glEnable(GL_CULL_FACE);

	if (VERBOSE) {
		printf("%f\n", t - lastT);
	}

	lastT = t;
	glutSwapBuffers();
}


void drawObject(mat4 transform, Model* model, GLuint p) {
	glUniformMatrix4fv(glGetUniformLocation(p, "transform"), 1, GL_TRUE, transform.m);
	DrawModel(model, p, "in_Position", "in_Normal", "in_TexCoord");
	printError("drawObject()");
}

void reshapeViewport(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	userCamera.projection = perspective(90, (GLfloat)w/(GLfloat)h, 0.1, 1000);
}

void handleMouse(int x, int y) {
	userCamera = rotateCameraByMouse(userCamera, x, y);
}

int main(int argc, char *argv[]) {
	glutInitContextVersion(3, 2);
	glutInit(&argc, argv);
	glutCreateWindow ("Let it snow");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeViewport);
	initKeymapManager();
	glutPassiveMotionFunc(handleMouse);
	init ();
	glutTimerFunc(16, &OnTimer, 0);
	glutMainLoop();
}
