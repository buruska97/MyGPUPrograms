#pragma once

class Terrain;

#include "texturedmesh.h"
#include "texture.h"

class Tree : public TexturedMesh
{
	static unsigned int trunkvao, leavesvao;

	static const int trunkResolution;
	static const float trunkRadius;
	static const float trunkHeight;

	static const int sphereResolution;
	static const float sphereRadius;

	static void TrunkCreate();
	static void LeavesCreate();

	static Texture trunkTexture;
	static Texture leavesTexture;

	Terrain& terrain;

	float age;
	static const float maxage; //1 fade in 10 growth 1 fadeout
	static const float minScale, maxScale;

public:
	virtual void Draw(const Program& program, const glm::mat4& VP);
	virtual void Update(float time, Scene& scene);
	static void Create();
	float getAge();

	Tree(Terrain& terrain, float age = 0.0f);
};