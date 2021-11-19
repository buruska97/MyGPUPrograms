#include "tree.h"

#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <math.h>

unsigned int Tree::trunkvao, Tree::leavesvao;

const int Tree::trunkResolution = 20;
const float Tree::trunkRadius = 0.05f;
const float Tree::trunkHeight = 0.5f;

const int Tree::sphereResolution = 20;
const float Tree::sphereRadius = 0.2f;

const float Tree::minScale = 0.02f, Tree::maxScale = 0.18f;

Texture Tree::trunkTexture, Tree::leavesTexture;

void Tree::Create()
{
	Tree::TrunkCreate();
	Tree::LeavesCreate();
}

void Tree::TrunkCreate()
{
	glGenVertexArrays(1, &trunkvao);
	glBindVertexArray(trunkvao);

	unsigned int vbo[3];
	glGenBuffers(3, &vbo[0]);

	std::vector<float> vertexCoords(trunkResolution * 6 + 6);
	std::vector<float> normalCoords(trunkResolution * 6 + 6);
	std::vector<float> texCoords(trunkResolution * 4 + 4);

	vertexCoords[0] = 0.0f;
	vertexCoords[1] = trunkRadius;
	vertexCoords[2] = 0.0f;
	vertexCoords[3] = 0.0f;
	vertexCoords[4] = trunkRadius;
	vertexCoords[5] = trunkHeight;

	normalCoords[0] = 0.0f;
	normalCoords[1] = 1.0f;
	normalCoords[2] = 0.0f;
	normalCoords[3] = 0.0f;
	normalCoords[4] = 1.0f;
	normalCoords[5] = 0.0f;

	texCoords[0] = 0.0f;
	texCoords[1] = 0.0f;
	texCoords[2] = 0.0f;
	texCoords[3] = 1.0f;


	for (int i = 0; i < trunkResolution; i++)
	{
		float angle = (float)(i + 1) / trunkResolution * 2 * glm::pi<float>();

		float xCoord = std::sinf(angle) * trunkRadius;
		float yCoord = std::cosf(angle) * trunkRadius;

		int index = 6 * (i + 1);
		int texindex = 4 * (i + 1);

		vertexCoords[index] = xCoord;
		vertexCoords[index + 1] = yCoord;
		vertexCoords[index + 2] = 0.0f;
		vertexCoords[index + 3] = xCoord;
		vertexCoords[index + 4] = yCoord;
		vertexCoords[index + 5] = trunkHeight;

		glm::vec3 normal = glm::normalize(glm::vec3(xCoord, yCoord, 0.0f));
		normalCoords[index] = normal.x;
		normalCoords[index + 1] = normal.y;
		normalCoords[index + 2] = normal.z;
		normalCoords[index + 3] = normal.x;
		normalCoords[index + 4] = normal.y;
		normalCoords[index + 5] = normal.z;

		float U = (float)(i + 1) / trunkResolution;

		texCoords[texindex] = U;
		texCoords[texindex + 1] = 0.0f;
		texCoords[texindex + 2] = U;
		texCoords[texindex + 3] = 1.0f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoords.size(), vertexCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalCoords.size(), normalCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	trunkTexture.create("textures/trunk.bmp");
	leavesTexture.create("textures/leaves.bmp");
}

void Tree::LeavesCreate()
{
	glGenVertexArrays(1, &leavesvao);
	glBindVertexArray(leavesvao);

	unsigned int vbo[3];
	glGenBuffers(3, &vbo[0]);

	std::vector<float> vertexCoords((sphereResolution * 6 + 6) * sphereResolution);
	std::vector<float> normalCoords((sphereResolution * 6 + 6) * sphereResolution);
	std::vector<float> texCoords((sphereResolution * 4 + 4) * sphereResolution);

	for (int i = 0; i < sphereResolution; i++)
	{
		float lowerX = (float)i / sphereResolution * sphereRadius * 2.0f - sphereRadius;
		float higherX = (float)(i + 1) / sphereResolution * sphereRadius * 2.0f - sphereRadius;

		float lowerRadius = std::sqrtf(sphereRadius * sphereRadius - lowerX * lowerX);
		float higherRadius = std::sqrtf(sphereRadius * sphereRadius - higherX * higherX);

		float lowerZ = (float)i / sphereResolution * sphereRadius * 2 + trunkHeight - sphereRadius;
		float higherZ = (float)(i + 1) / sphereResolution * sphereRadius * 2 + trunkHeight - sphereRadius;

		int offset = i * (sphereResolution * 6 + 6);
		int texOffset = i * (sphereResolution * 4 + 4);

		vertexCoords[offset] = 0.0f;
		vertexCoords[offset + 1] = lowerRadius;
		vertexCoords[offset + 2] = lowerZ;
		vertexCoords[offset + 3] = 0.0f;
		vertexCoords[offset + 4] = higherRadius;
		vertexCoords[offset + 5] = higherZ;

		glm::vec3 lowerNormal = glm::normalize(glm::vec3(0.0f, lowerRadius, lowerZ - trunkHeight));
		glm::vec3 higherNormal = glm::normalize(glm::vec3(0.0f, higherRadius, higherZ - trunkHeight));

		normalCoords[offset] = lowerNormal.x;
		normalCoords[offset + 1] = lowerNormal.y;
		normalCoords[offset + 2] = lowerNormal.z;
		normalCoords[offset + 3] = higherNormal.x;
		normalCoords[offset + 4] = higherNormal.y;
		normalCoords[offset + 5] = higherNormal.z;

		texCoords[texOffset] = 0.0f;
		texCoords[texOffset + 1] = (float)i / sphereResolution;
		texCoords[texOffset + 2] = 0.0f;
		texCoords[texOffset + 3] = (float)(i + 1) / sphereResolution;

		for (int j = 0; j < sphereResolution; j++)
		{
			float angle = (float)(j + 1) / sphereResolution * 2 * glm::pi<float>();

			float lowerXCoord = std::sinf(angle) * lowerRadius;
			float lowerYCoord = std::cosf(angle) * lowerRadius;
			float higherXCoord = std::sinf(angle) * higherRadius;
			float higherYCoord = std::cosf(angle) * higherRadius;

			int index = 6 * (j + 1);
			int texindex = 4 * (j + 1);

			vertexCoords[offset + index] = lowerXCoord;
			vertexCoords[offset + index + 1] = lowerYCoord;
			vertexCoords[offset + index + 2] = lowerZ;
			vertexCoords[offset + index + 3] = higherXCoord;
			vertexCoords[offset + index + 4] = higherYCoord;
			vertexCoords[offset + index + 5] = higherZ;

			glm::vec3 lowerNormal = glm::normalize(glm::vec3(lowerXCoord, lowerYCoord, lowerZ - trunkHeight));
			glm::vec3 higherNormal = glm::normalize(glm::vec3(higherXCoord, higherYCoord, higherZ - trunkHeight));

			normalCoords[offset + index] = lowerNormal.x;
			normalCoords[offset + index + 1] = lowerNormal.y;
			normalCoords[offset + index + 2] = lowerNormal.z;
			normalCoords[offset + index + 3] = higherNormal.x;
			normalCoords[offset + index + 4] = higherNormal.y;
			normalCoords[offset + index + 5] = higherNormal.z;

			float U = (float)(j + 1) / sphereResolution;

			texCoords[texOffset + texindex] = U;
			texCoords[texOffset + texindex + 1] = (float)i / sphereResolution;
			texCoords[texOffset + texindex + 2] = U;
			texCoords[texOffset + texindex + 3] = (float)(i + 1) / sphereResolution;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoords.size(), vertexCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalCoords.size(), normalCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Tree::Draw(const Program& program, const glm::mat4& VP)
{
	TexturedMesh::Draw(program, VP);
	
	glBindVertexArray(trunkvao);
	program.setUniformTexture("texture", trunkTexture.getTexId(), 0);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, trunkResolution * 2 + 2);

	glBindVertexArray(leavesvao);
	program.setUniformTexture("texture", leavesTexture.getTexId(), 0);
	for (int i = 0; i < sphereResolution; i++)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, (sphereResolution * 2 + 2) * i, (sphereResolution * 2 + 2));
	}
	glBindVertexArray(0);
}

void Tree::Update(float time, Scene& scene)
{
	age += time;
	if (age < 1.0f)
	{
		opacity = age;
		scale = glm::vec3(minScale);
	}
	else if (age < 11.0f)
	{
		float lambda = (age - 1.0f) / 10.0f;
		scale = glm::vec3(maxScale * lambda + minScale * (1.0f - lambda));
	}
	else if (age < 12.0f)
	{
		scale = glm::vec3(maxScale);
		opacity = 12.0f - age;
	}
	else
		destroyed = true;
}

Tree::Tree(Terrain& terrain, float age) : terrain(terrain), age(age) {}

float Tree::getAge()
{
	return age;
}

