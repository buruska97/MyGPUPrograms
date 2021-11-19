#pragma once

#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <algorithm>

class Light
{
	float last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
	int arcball_on = false;

	glm::vec3 get_arcball_vector(float x, float y);
	glm::mat4 rotationMatrix;

	float sensitivity;

public:
	Light();
	void mouseButtonChanged(int button, int state, float x, float y);
	void mouseDragged(glm::vec2 mouseDelta);
	void onIdle(glm::mat4 V);

	glm::mat4 GetRotationMatrix();
};