#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "texturedmesh.h"


class Triangle : public TexturedMesh
{
	static unsigned int vao;

public:
	virtual void Draw(const Program& program, const glm::mat4& VP);
	virtual void Update(float time);
	static void Create();
};