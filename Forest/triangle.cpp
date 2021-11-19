#include "triangle.h"

void Triangle::Create()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo[2];
	glGenBuffers(2, &vbo[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	float vertexCoords[] = { -0.5f, -0.5f, -1.5f, 0, 0.5f, -1.5f, 0.5f, -0.5f, -1.5f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	float texCoords[] = { 0, 0, 0.5f, 1, 1, 0 };
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Triangle::Update(float time)
{

}

void Triangle::Draw(const Program& program, const glm::mat4& VP)
{
	glBindVertexArray(vao);
	glm::mat4 MVP =  VP * M();

	program.setUniformMat4("MVP", MVP);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

unsigned int Triangle::vao = 0;