#pragma once
#include "program.h"

class BackgroundQuad
{
	static unsigned int vao;

public:
	static void Create();
	void Draw(const Program& program, float pitch);
};