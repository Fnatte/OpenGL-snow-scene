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

#define VERBOSE 0


// vertex array object
unsigned int vertexArrayObjID;
GLuint program;
GLuint skyboxProgram;
GLuint instancingProgram;
GLuint groundProgram;

Model *octagon;
Model *skybox;
Model *plane;
mat4 transCubes;
mat4 transCubes1;
mat4 transCubes2;
mat4 transGround;

GLuint skyTexture;
GLuint groundTexture;

GLfloat lastT = 0;

int nrInstances = 20;


void init(void) {
	dumpInfo();

	initCamera();

	octagon = LoadModelPlus("./models/octagon.obj");
	skybox = LoadModelPlus("./models/skybox.obj");
	plane = LoadModelPlus("./models/plane2.obj");

	transGround = T(0, 0, 0);
	transCubes = T(-2.2, -2.3, 10.2);
	transCubes1 = T(88, -2.3, 10.2);
	transCubes2 = T(-88, -2.3, 10.2);

	LoadTGATextureSimple("./textures/SkyBox512.tga", &skyTexture);
	LoadTGATextureSimple("./textures/red.tga", &groundTexture);

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

	glUseProgram(skyboxProgram);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxProgram, "texUnit"), 0);

	glUseProgram(instancingProgram);
	glUniform1i(glGetUniformLocation(instancingProgram, "texUnit"), 0);
	setupInstancedVertexAttributes(instancingProgram, nrInstances);

	glUseProgram(groundProgram);
	glUniform1i(glGetUniformLocation(groundProgram, "texUnit"), 0);
	initializeGround(plane, groundProgram, groundTexture);

	printError("init(): End");
}

void OnTimer(int value) {
	glutPostRedisplay();
	glutTimerFunc(16, &OnTimer, value);
	printError("OnTimer()");
}

void display(void) {
	printError("pre display");
	mat4 projectionViewMatrix = getProjectionViewMatrix();

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	glBindTexture(GL_TEXTURE_2D, skyTexture);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	mat4 cameraTrans = T(getCameraPos().x, getCameraPos().y, getCameraPos().z);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "transform"), 1, GL_TRUE, cameraTrans.m);
	DrawModel(skybox, skyboxProgram, "in_Position", NULL, "in_TexCoord");
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Draw instances
	glUseProgram(instancingProgram);
	glUniformMatrix4fv(glGetUniformLocation(instancingProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	drawModelInstanced(octagon, instancingProgram, nrInstances, t, transCubes);

	// Draw ground
	drawGround(Mult(projectionViewMatrix, transGround));

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
	glutReshapeFunc(reshape);
	initKeymapManager();
	glutPassiveMotionFunc(handleMouse);
	init ();
	glutTimerFunc(16, &OnTimer, 0);
	glutMainLoop();
}
