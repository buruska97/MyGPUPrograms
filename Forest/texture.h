#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

class Texture
{
	std::vector<glm::vec4> load(std::string path, bool transparent, int& width, int& height);
	unsigned int textureId;

public:
	
	Texture();
	Texture(std::string path, bool transparent = false);
	Texture(int width, int height, const std::vector<glm::vec4>& image, int sampling = GL_LINEAR);
	Texture(const Texture& texture) = delete;
	void operator=(const Texture& texture) = delete;

	void create(std::string path, bool transparent = false);
	void create(int width, int height, const std::vector<glm::vec4>& image, int sampling = GL_LINEAR);

	GLuint getTexId();

	~Texture();
};