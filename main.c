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

#define near 1.0
#define far 1000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

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

mat4 projectionMatrix;
mat4 lookMatrix;
vec3 cameraPos;
vec3 cameraTarget;
vec3 cameraNormal;
vec3 cameraDirection;

int nrInstances = 20;

void init(void) {
	cameraPos = (vec3){1.5f, 20.0f, -10.0f};
	cameraTarget = (vec3){10.0f, 5.0f, 0.0f};
	cameraNormal = (vec3){0.0f, 1.0f, 0.0f};
	lookMatrix = lookAtv(cameraPos, cameraTarget, cameraNormal);

	dumpInfo();

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

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	projectionMatrix = perspective(90, (GLfloat)w/(GLfloat)h, 0.1, 1000);
}

void display(void) {
	printError("pre display");
	cameraPos = moveCameraOnKeyboard(cameraPos, cameraNormal, cameraDirection);
	cameraTarget = moveCameraOnKeyboard(cameraTarget, cameraNormal, cameraDirection);

	lookMatrix = lookAtv(cameraPos, cameraTarget, cameraNormal);
	mat4 projectionViewMatrix = Mult(projectionMatrix, lookMatrix);


	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	glBindTexture(GL_TEXTURE_2D, skyTexture);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "transform"), 1, GL_TRUE, T(cameraPos.x, cameraPos.y, cameraPos.z).m);
	DrawModel(skybox, skyboxProgram, "in_Position", NULL, "in_TexCoord");
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Draw instances
	glUseProgram(instancingProgram);
	glUniformMatrix4fv(glGetUniformLocation(instancingProgram, "projectionViewMatrix"), 1, GL_TRUE, projectionViewMatrix.m);
	drawModelInstanced(octagon, instancingProgram, nrInstances, t, transCubes);

	// Draw ground
	glUniformMatrix4fv(glGetUniformLocation(groundProgram, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
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


void handleMouse(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	cameraTarget = (vec3)
		{cos((float)x / width * M_PI * 2) * sin((float)y / height * M_PI),
		 -(float)y / height + 0.5,
		 sin((float)x / width * M_PI * 2) * sin((float)y / height * M_PI)};
	cameraTarget = VectorAdd(cameraTarget, cameraPos);

	lookMatrix = lookAtv(cameraPos, cameraTarget, cameraNormal);
	cameraDirection = Normalize(VectorSub(cameraTarget, cameraPos));
	printError("handleMouse()");
}


int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutCreateWindow ("Let it snow");
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
