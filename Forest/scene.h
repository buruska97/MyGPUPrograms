#pragma once

#include <list>
#include <memory>

#include "texturedmesh.h"
#include "terrain.h"
#include "tree.h"
#include "light.h"
#include "bgquad.h"
#include "camera.h"

class Scene
{
	std::list<std::shared_ptr<TexturedMesh>> meshes;
	Program program, bgProgram;
	Camera& camera;
	Light& light;
	BackgroundQuad bgquad;

	float prevT;

public:
	static void onInitialization();
	void render();
	void onIdle(float dt);

	void addObject(std::shared_ptr<TexturedMesh> mesh);

	Scene(Camera& camera, Light& light);
};