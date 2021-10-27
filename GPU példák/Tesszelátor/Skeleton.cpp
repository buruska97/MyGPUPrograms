// Skeleton.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <algorithm>

#include "shader.hpp"
#include "texture.hpp"
#include "DebugOpenGL.hpp"

const unsigned int windowWidth = 600;
const unsigned int windowHeight = 600;

Shader shader;

GLuint vao;
GLuint vertexBuffer;

int minTessLevel = 1;
int maxTessLevel = 8;
float th = 0.9f;

float* vertices;
const unsigned int planesize = 10;
unsigned int verticesSize;

struct vec2
{
	float x, y;
};


void onInitialization()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Cannot initialize GLEW\n");
		exit(-1);
	}
	glGetError();

	DebugOpenGL::init();
	DebugOpenGL::enableLowSeverityMessages(false);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	shader.loadShader(GL_VERTEX_SHADER, "../shaders/tess.vert");
	shader.loadShader(GL_TESS_CONTROL_SHADER, "../shaders/tess.tc");
	shader.loadShader(GL_TESS_EVALUATION_SHADER, "../shaders/tess.te");
	shader.loadShader(GL_FRAGMENT_SHADER, "../shaders/tess.frag");
	shader.compile();

	verticesSize = 4 * 3 * planesize * planesize;
	vertices = new float [verticesSize];

	int arrayIndex = 0;

	for (int i = 0; i < planesize; i++)
	{
		for (int j = 0; j < planesize; j++)
		{
			vec2 p1, p2;
			float step = 2.0f / planesize;

			p1.x = -1.0f + i * step;
			p1.y = -1.0f + j * step;
			p2.x = -1.0f + (i + 1) * step;
			p2.y = -1.0f + (j + 1) * step;

			vertices[arrayIndex++] = p1.x;
			vertices[arrayIndex++] = p1.y;
			vertices[arrayIndex++] = -1.5f;

			vertices[arrayIndex++] = p1.x;
			vertices[arrayIndex++] = p2.y;
			vertices[arrayIndex++] = -1.5f;

			vertices[arrayIndex++] = p2.x;
			vertices[arrayIndex++] = p2.y;
			vertices[arrayIndex++] = -1.5f;

			vertices[arrayIndex++] = p2.x;
			vertices[arrayIndex++] = p1.y;
			vertices[arrayIndex++] = -1.5f;

		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 MV = glm::mat4(1.0f);
	MV = glm::rotate(MV, 1.04f, glm::vec3(0.0f, 1.0f, 0.0f)); //60 degrees
	MV = glm::translate(MV, glm::vec3(1.5f, 0.0f, -0.0f));

	glm::mat4 P = glm::perspective(55.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	//glm::mat4 P = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 MVP = P * MV;

	

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shader.enable();
	shader.setUniform1f("th", th);
	shader.setUniform1i("maxTessLevel", maxTessLevel);
	shader.setUniform1i("minTessLevel", minTessLevel);
	shader.setUniformMat4("MVP", MVP);
	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, verticesSize / 3);
	glBindVertexArray(0);
	shader.disable();
	
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int pX, int pY) {
	switch (key)
	{
	case 27:
		glutExit();
		break;

	case '[':
		minTessLevel = std::max(1, minTessLevel - 1);
		break;

	case ']':
		minTessLevel = std::min(maxTessLevel, minTessLevel + 1);
		th += 0.01f;
		break;

	case '{':
		maxTessLevel = std::max(minTessLevel, maxTessLevel - 1);
		break;

	case '}':
		maxTessLevel = std::min(64, maxTessLevel + 1);
		break;
	}
}

void onIdle()
{
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
	glutKeyboardFunc(onKeyboard);
	glutIdleFunc(onIdle);
	glutMainLoop();

    return 0;
}

