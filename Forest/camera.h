#pragma once

#include <set>

#include <glm/glm.hpp>
#include <GL/freeglut.h>


class Camera
{
	glm::vec3 position;
	//glm::mat4 rotationMatrix;
	float roll, pitch, yaw;

	const glm::vec4 defAhead = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	const glm::vec4 defRight = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	const glm::vec4 defUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	glm::vec3 ahead, right, up;

	float speed;
	float sensitivity;

	
	glm::mat4 perspectiveMatrix() const;

	std::set<unsigned char> keysPressed;

	bool leftMouseButtonDown;

public:
	Camera();

	glm::mat4 rotationMatrix() const;
	glm::mat4 viewProjectMatrix() const;
	glm::mat4 viewMatrix() const;

	void keyPressed(unsigned char key);
	void keyReleased(unsigned char key);

	void mouseDragged(glm::vec2 mouseDelta);
	void mouseButtonChanged(int button, int state);

	void Update(float dt);

	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();

	float GetPitch();
};