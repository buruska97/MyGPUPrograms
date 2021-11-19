// Forest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <ctime>

#include "camera.h"
#include "terrain.h"
#include "light.h"
#include "scene.h"


Light light;
Scene* scene;
Camera camera;

const unsigned int windowWidth = 600;
const unsigned int windowHeight = 600;

void onInitialization()
{
	srand(time(0));
    glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		printf("Cannot initialize GLEW\n");
		exit(-1);
	}
	glGetError();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Scene::onInitialization();
	scene = new Scene(camera, light);
	
	auto terrain = std::make_shared<Terrain>();
	terrain->SetRotation(glm::vec3(glm::pi<float>() / 2, 0.0f, 0.0f));
	terrain->SetPosition(glm::vec3(-0.5f, -0.5f, 0.0f));
	scene->addObject(terrain);

	for (int i = 0; i < 60; i++)
		terrain->spawnRandomTree(*scene);
}

void onDisplay()
{
	scene->render();

	glutSwapBuffers();
}


void onKeyPressed(unsigned char key, int pX, int pY) {
	switch (key)
	{
	case 27:
		glutExit();
		break;
	default:
		camera.keyPressed(key);
	}
}

void onKeyReleased(unsigned char key, int pX, int pY) {
	camera.keyReleased(key);
}


glm::vec2 prevMousePos = glm::vec2(-2.0f);

void onMotion(int x, int y)
{
	glm::vec2 currentMousePos = glm::vec2(float(x) / windowWidth, 1.0 - float(y) / windowHeight);
	light.mouseDragged(currentMousePos);
	
	
	if (prevMousePos.x > -1.5f)
	{
		camera.mouseDragged(currentMousePos - prevMousePos);
	}
	prevMousePos = currentMousePos;

	glutPostRedisplay();
}

void onMouseButton(int button, int state, int x, int y)
{
	camera.mouseButtonChanged(button, state);

	glm::vec2 currentMousePos = glm::vec2(float(x) / windowWidth, 1.0 - float(y) / windowHeight);
	light.mouseButtonChanged(button, state, currentMousePos.x, currentMousePos.y);


	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		prevMousePos = glm::vec2(-2.0f);
	}
}

float prevT = 0.0f;
void onIdle()
{
	long long time = glutGet(GLUT_ELAPSED_TIME);
	float t = time / 1000.f;
	float dt = t - prevT;
	prevT = t;

	scene->onIdle(dt);
	camera.Update(dt);
	light.onIdle(camera.rotationMatrix());

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glewExperimental = true;
	glewInit();

	printf("GL Vendor    : %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer  : %s\n", glGetString(GL_RENDERER));
	printf("GL Version (string)  : %s\n", glGetString(GL_VERSION));
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	onInitialization();
	glutDisplayFunc(onDisplay);
	
	glutKeyboardFunc(onKeyPressed);
	glutKeyboardUpFunc(onKeyReleased);
	glutMotionFunc(onMotion);
	glutMouseFunc(onMouseButton);
	glutIdleFunc(onIdle);

	glutMainLoop();

	return 0;
}