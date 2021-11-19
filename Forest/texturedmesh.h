#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>

class Scene;

#include "program.h"

class TexturedMesh
{
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	float opacity;

	bool destroyed = false;

public:

	TexturedMesh();

	virtual void Draw(const Program& program, const glm::mat4& VP);
	virtual void Update(float time, Scene& scene) = 0;
	void SetPosition(const glm::vec3&);
	void SetRotation(const glm::vec3&);
	void SetScale(const glm::vec3&);

	bool isDestroyed();
	void Destroy();

	glm::mat4 M() const;
};