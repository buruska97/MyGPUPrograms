#pragma once

#include "terrain.h"
#include <math.h>
#include <vector>
#include <algorithm>

#include "scene.h"



void Terrain::Create()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo[3];
	glGenBuffers(3, &vbo[0]);

	for (int i = 0; i < 5; i++)
	{
		freq[i] = getRandomFloat(10.0f, 50.f);
		faze[i] = getRandomFloat(0.0f, 3.14f);
		amp[i] = getRandomFloat(0.002f, 0.03f);
	}

	std::vector<float> vertexCoords(2 * 3 * 3 * size *size);
	std::vector<float> normalCoords(2 * 3 * 3 * size * size);
	std::vector<float> texCoords(2 * 3 * 2 * size * size);

	glm::vec3 vertexMatrix[size+1][size+1];
	glm::vec3 normalMatrix[size+1][size+1];

	for (int i = 0; i <= size; i++)
		for (int j = 0; j <= size; j++)
		{
			vertexMatrix[i][j] = glm::vec3((float)i / size, (float)j / size, myHeightMap((float)i / size, (float)j / size));
		}

	for (int i = 0; i <= size; i++)
		for (int j = 0; j <= size; j++)
		{
			normalMatrix[i][j] = myNormalMap((float)i / size, (float)j / size);
		}

	int index = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			vertexCoords[index++] = vertexMatrix[i][j].x;
			vertexCoords[index++] = vertexMatrix[i][j].y;
			vertexCoords[index++] = vertexMatrix[i][j].z;

			vertexCoords[index++] = vertexMatrix[i + 1][j].x;
			vertexCoords[index++] = vertexMatrix[i + 1][j].y;
			vertexCoords[index++] = vertexMatrix[i + 1][j].z;

			vertexCoords[index++] = vertexMatrix[i][j + 1].x;
			vertexCoords[index++] = vertexMatrix[i][j + 1].y;
			vertexCoords[index++] = vertexMatrix[i][j + 1].z;

			vertexCoords[index++] = vertexMatrix[i + 1][j].x;
			vertexCoords[index++] = vertexMatrix[i + 1][j].y;
			vertexCoords[index++] = vertexMatrix[i + 1][j].z;

			vertexCoords[index++] = vertexMatrix[i][j + 1].x;
			vertexCoords[index++] = vertexMatrix[i][j + 1].y;
			vertexCoords[index++] = vertexMatrix[i][j + 1].z;

			vertexCoords[index++] = vertexMatrix[i + 1][j + 1].x;
			vertexCoords[index++] = vertexMatrix[i + 1][j + 1].y;
			vertexCoords[index++] = vertexMatrix[i + 1][j + 1].z;
		}

	index = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			normalCoords[index++] = normalMatrix[i][j].x;
			normalCoords[index++] = normalMatrix[i][j].y;
			normalCoords[index++] = normalMatrix[i][j].z;

			normalCoords[index++] = normalMatrix[i + 1][j].x;
			normalCoords[index++] = normalMatrix[i + 1][j].y;
			normalCoords[index++] = normalMatrix[i + 1][j].z;

			normalCoords[index++] = normalMatrix[i][j + 1].x;
			normalCoords[index++] = normalMatrix[i][j + 1].y;
			normalCoords[index++] = normalMatrix[i][j + 1].z;

			normalCoords[index++] = normalMatrix[i + 1][j].x;
			normalCoords[index++] = normalMatrix[i + 1][j].y;
			normalCoords[index++] = normalMatrix[i + 1][j].z;

			normalCoords[index++] = normalMatrix[i][j + 1].x;
			normalCoords[index++] = normalMatrix[i][j + 1].y;
			normalCoords[index++] = normalMatrix[i][j + 1].z;

			normalCoords[index++] = normalMatrix[i + 1][j + 1].x;
			normalCoords[index++] = normalMatrix[i + 1][j + 1].y;
			normalCoords[index++] = normalMatrix[i + 1][j + 1].z;
		}

	index = 0;
	for (int i = 0; i < 400; i++)
	{
		texCoords[index++] = 0.0f;
		texCoords[index++] = 0.0f;

		texCoords[index++] = 1.0f;
		texCoords[index++] = 0.0f;

		texCoords[index++] = 0.0f;
		texCoords[index++] = 1.0f;

		texCoords[index++] = 1.0f;
		texCoords[index++] = 0.0f;

		texCoords[index++] = 0.0f;
		texCoords[index++] = 1.0f;

		texCoords[index++] = 1.0f;
		texCoords[index++] = 1.0f;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	texture.create("textures/grass.bmp");
}

void Terrain::Update(float time, Scene& scene)
{
	cummulativeTime += time;
	if (cummulativeTime > 2.0f)
	{
		iterateOnFreeSpaces(scene);
		cummulativeTime -= 2.0f;
	}
}

void Terrain::Draw(const Program& program, const glm::mat4& VP)
{
	TexturedMesh::Draw(program, VP);
	glBindVertexArray(vao);
	
	program.setUniformTexture("texture", texture.getTexId(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3 * size * size);
	glBindVertexArray(0);
}

float Terrain::myHeightMap(float x, float y)
{

	float xComponent = 0.0f;
	float yComponent = 0.0f;
	for (int i = 0; i < numOfSins; i++)
	{
		xComponent += amp[2 * i] * std::sinf(freq[2 * i] * x + faze[2 * i]);
		yComponent += amp[2 * i + 1] * std::sinf(freq[2 * i + 1] * y + faze[2 * i + 1]);
	}

	return xComponent + yComponent;
}

glm::vec3 Terrain::myNormalMap(float x, float y)
{

	float xDerivate = 0.0f;
	float yDerivate = 0.0f;
	for (int i = 0; i < numOfSins; i++)
	{
		xDerivate += amp[2 * i] * freq[2 * i] * std::cosf(freq[2 * i] * x + faze[2 * i]);
		yDerivate += amp[2 * i + 1] * freq[2 * i + 1] * std::cosf(freq[2 * i + 1] * y + faze[2 * i + 1]);
	}

	glm::vec3 xTan = glm::vec3(1.0f, 0.0f, xDerivate);
	glm::vec3 yTan = glm::vec3(0.0f, 1.0f, yDerivate);

	return -glm::normalize(glm::cross(xTan, yTan));
}

unsigned int Terrain::vao = 0;
Texture Terrain::texture;
float Terrain::freq[2 * numOfSins], Terrain::faze[2 * numOfSins], Terrain::amp[2 * numOfSins];

void Terrain::spawnRandomTree(Scene& scene)
{
	bool spawned = false;
	while (!spawned)
	{
		int i = rand() % (size - 1);
		int j = rand() % (size - 1);

		auto p = treegrid[i][j].lock();

		if (!p)
		{
			float r = getRandomFloat(1.0f, 11.0f);
			spawnTreeOnPoint(scene, i, j, r);
			spawned = true;
		}
	}
}

void Terrain::spawnTreeOnPoint(Scene& scene, int i, int j, float age)
{
	auto newTree = std::make_shared<Tree>(*this, age);

	float x = (float)(i + 1) / size;
	float y = (float)(j + 1) / size;
	float z = myHeightMap((float)(i + 1) / size, (float)(j + 1) / size);
	newTree->SetPosition(glm::vec3(this->M() * glm::vec4(x, y, z, 1.0f)));
	newTree->SetScale(glm::vec3(0.5f));
	newTree->SetRotation(glm::vec3(-glm::pi<float>() / 2, 0.0f, 0.0f));

	scene.addObject(newTree);
	treegrid[i][j] = newTree;

	riotControl();
}

void Terrain::iterateOnFreeSpaces(Scene& scene)
{
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - 1; j++)
		{
			auto p = treegrid[i][j].lock();

			if (!p)
			{
				if (rand() % 10 == 0)
				{
					spawnTreeOnPoint(scene, i, j);
				}
			}
		}
}

void Terrain::riotControl()
{
	int counter = 0;
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - 1; j++)
		{
			auto p = treegrid[i][j].lock();

			if (p)
			{
				counter++;
			}
		}

	std::vector<std::shared_ptr<Tree>> trees;
	if (counter >= 100)
	{
		for (int i = 0; i < size - 1; i++)
			for (int j = 0; j < size - 1; j++)
			{
				auto p = treegrid[i][j].lock();

				if (p)
				{
					trees.push_back(p);
				}
			}

		std::sort(trees.begin(), trees.end(), [](auto a, auto b) { return a->getAge() > b->getAge();});

		for (int i = 0; i < 50; i++)
			trees[i]->Destroy();
	}
}

Terrain::Terrain() : cummulativeTime(0.0f) {}

float Terrain::getRandomFloat(float b, float e)
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	r *= e - b;
	r += b;

	return r;
}