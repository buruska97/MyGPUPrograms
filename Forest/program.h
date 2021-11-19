#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>


class Program
{
public:
	bool loadShader(GLuint type, std::string filename);
	bool compile();
	bool enable();
	bool disable();

	bool setUniformMat4(std::string name, const glm::mat4& m) const;
	bool setUniformTexture(std::string name, GLuint textureID, GLuint unitID) const;
	bool setUniformVec4(std::string name, glm::vec4 vec4) const;
	bool setUniformFloat(std::string name, float value) const;

private:
	GLuint programID;

	std::string vertexShaderPath;
	std::string vertexShaderSource;
	GLuint vertexShaderID;

	std::string fragmentShaderPath;
	std::string fragmentShaderSource;
	GLuint fragmentShaderID;

	bool loadShaderSource(std::string& source, std::string filename);
};