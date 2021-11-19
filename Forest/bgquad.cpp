#include "bgquad.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

void BackgroundQuad::Create()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	float vertexCoords[] = { -1.0f, -1.0f,
							-1.0f, 1.0f,
							1.0f, -1.0f,
							1.0f, 1.0f };

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, vertexCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
}

void BackgroundQuad::Draw(const Program& program, float pitch)
{
	program.setUniformFloat("pitch", pitch);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int BackgroundQuad::vao;