#pragma once

#include "texturedmesh.h"
#include "texture.h"
#include "tree.h"

#include <memory>

class Terrain : public TexturedMesh
{
	static unsigned int vao;

	static const int numOfSins = 5;
	static float freq[], faze[], amp[];
	

	static float myHeightMap(float x, float y);
	static glm::vec3 myNormalMap(float x, float y);

	static constexpr const int size = 20;

	static Texture texture;

	std::weak_ptr<Tree> treegrid[size - 1][size - 1];
	float cummulativeTime;

	void spawnTreeOnPoint(Scene& scene, int i, int j, float age = 0.0f);
	void iterateOnFreeSpaces(Scene& scene);

	void riotControl();

public:
	virtual void Draw(const Program& program, const glm::mat4& VP);
	virtual void Update(float time, Scene& scene);
	static void Create();

	void spawnRandomTree(Scene& scene);
	static float getRandomFloat(float b, float e);

	Terrain();
};