#include "camera.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

Camera::Camera() : position(0.0f), roll(0.0f), pitch(0.0f), yaw(0.0f), speed(2.0f), sensitivity(0.5f) { Update(0); }

glm::mat4 Camera::rotationMatrix() const
{
	glm::mat4 m = glm::mat4(1.0f);
	m = glm::rotate(m, roll, glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::rotate(m, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	m = glm::rotate(m, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	
	return m;
}

glm::mat4 Camera::perspectiveMatrix() const
{
	return glm::perspective(glm::radians(55.0f), 1.0f, 0.1f, 1000.0f);
}

glm::mat4 Camera::viewProjectMatrix() const
{
	return perspectiveMatrix() * viewMatrix();
}

glm::mat4 Camera::viewMatrix() const
{
	glm::mat4 V = glm::inverse(glm::translate(position) * rotationMatrix());
	return V;
}

void Camera::keyPressed(unsigned char key)
{
	keysPressed.insert(key);
}

void Camera::keyReleased(unsigned char key)
{
	keysPressed.erase(key);
}

std::ostream& operator<<(std::ostream& os, glm::vec3 vec)
{
	os << vec.x << " " << vec.y << " " << vec.z;
	return os;
}

void Camera::Update(float dt)
{
	ahead = glm::vec3(rotationMatrix() * defAhead);
	right = glm::vec3(rotationMatrix() * defRight);
	up = glm::vec3(rotationMatrix() * defUp);

	if (keysPressed.find('p') != keysPressed.end())
	{
		std::cout << ahead << " " << position << "\n";
	}

	if (keysPressed.find('w') != keysPressed.end())
	{
		position += ahead * (speed * dt);
	}
	if (keysPressed.find('s') != keysPressed.end())
	{
		position -= ahead * (speed * dt);
	}
	if (keysPressed.find('a') != keysPressed.end())
	{
		position -= right * (speed * dt);
	}
	if (keysPressed.find('d') != keysPressed.end())
	{
		position += right * (speed * dt);
	}
}

void Camera::mouseDragged(glm::vec2 mouseDelta)
{
	if (leftMouseButtonDown)
	{
		yaw += mouseDelta.x * sensitivity;
		pitch -= mouseDelta.y * sensitivity;
	}
	if (pitch > glm::pi<float>() / 2.0f) {
		pitch = glm::pi<float>() / 2.0f;
	}
	if (pitch < -glm::pi<float>() / 2.0f) {
		pitch = -glm::pi<float>() / 2.0f;
	}
}

void Camera::mouseButtonChanged(int button, int state)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
			leftMouseButtonDown = false;
		if (state == GLUT_DOWN)
			leftMouseButtonDown = true;
	}
}

void Camera::SetPosition(glm::vec3 pos)
{
	position = glm::vec3(pos);
}

glm::vec3 Camera::GetPosition()
{
	return position;
}

float Camera::GetPitch()
{
	return pitch;
}