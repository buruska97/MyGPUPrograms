#include "scene.h"

#include <memory>

void Scene::onInitialization()
{
	Terrain::Create();
	Tree::Create();
	BackgroundQuad::Create();
}

Scene::Scene(Camera& camera, Light& light) : camera(camera), light(light), prevT(0.0f)
{
	program.loadShader(GL_VERTEX_SHADER, "shaders/vert.glsl");
	program.loadShader(GL_FRAGMENT_SHADER, "shaders/frag.glsl");

	camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	program.compile();

	bgProgram.loadShader(GL_VERTEX_SHADER, "shaders/quadvert.glsl");
	bgProgram.loadShader(GL_FRAGMENT_SHADER, "shaders/quadfrag.glsl");

	bgProgram.compile();
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	
	bgProgram.enable();
	bgquad.Draw(bgProgram, camera.GetPitch());
	bgProgram.disable();

	glm::mat4 VP = camera.viewProjectMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	program.enable();
	glm::vec4 lightDir = light.GetRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	program.setUniformVec4("light", lightDir);
	program.setUniformVec4("wEye", glm::vec4(camera.GetPosition(), 1.0f));

	for (auto pMesh : meshes)
	{
		pMesh->Draw(program, VP);
	}

	program.disable();
}

void Scene::onIdle(float dt)
{
	for (auto pMesh : meshes)
	{
		pMesh->Update(dt, *this);
	}
	meshes.remove_if([](auto element) {return element->isDestroyed(); });
}

void Scene::addObject(std::shared_ptr<TexturedMesh> mesh)
{
	meshes.push_back(mesh);
}