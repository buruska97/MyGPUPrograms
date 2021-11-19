#include "program.h"

bool Program::loadShader(GLuint type, std::string filename)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (loadShaderSource(vertexShaderSource, filename))
		{
			vertexShaderPath = filename;
		}
		break;

	case GL_FRAGMENT_SHADER:
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (loadShaderSource(fragmentShaderSource, filename))
		{
			fragmentShaderPath = filename;
		}
		break;

	default:
		return false;
	}

	return true;
}

bool Program::loadShaderSource(std::string& source, std::string filename)
{
	std::ifstream shaderStream(filename, std::ios::in);
	if (shaderStream.is_open())
	{
		if (source.length() > 0) 
			source.clear();
		std::string line = "";
		while (std::getline(shaderStream, line))
		{
			source += "\n" + line;
		}
		shaderStream.close();
	}
	else
	{
		std::cerr << "Can't load shader file: " << filename << std::endl;
		return false;
	}
	return true;
}

bool Program::compile()
{
	GLint result = GL_FALSE;
	int InfoLogLength;

	
	const char* vertexShaderSourcePtr = vertexShaderSource.c_str();
	glShaderSource(vertexShaderID, 1, &vertexShaderSourcePtr, NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> vertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);
		std::cout << "Vertex shader: " << vertexShaderErrorMessage.data() << std::endl;
	}
	

	const char* fragmentShaderSourcePtr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSourcePtr, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> fragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(fragmentShaderID, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		std::cout << "Fragment shader: " << fragmentShaderErrorMessage.data() << std::endl;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> programErrorMessage(InfoLogLength);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &programErrorMessage[0]);
		std::cout << "Link: " << programErrorMessage.data() << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return true;
}

bool Program::enable()
{
	glUseProgram(programID);
	return true;
}

bool Program::disable()
{
	glUseProgram(0);
	return true;
}

bool Program::setUniformMat4(std::string name, const glm::mat4& m) const
{
	GLuint id = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(id, 1, GL_FALSE, &m[0][0]);
	return true;
}

bool Program::setUniformTexture(std::string name, GLuint textureID, GLuint unitID) const
{
	GLuint id = glGetUniformLocation(programID, name.c_str());
	glActiveTexture(GL_TEXTURE0 + unitID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(id, unitID);
	return true;
}

bool Program::setUniformVec4(std::string name, glm::vec4 vec) const
{
	GLuint id = glGetUniformLocation(programID, name.c_str());
	glUniform4f(id, vec.x, vec.y, vec.z, vec.w);
	return true;
}

bool Program::setUniformFloat(std::string name, float value) const
{
	GLuint id = glGetUniformLocation(programID, name.c_str());
	glUniform1f(id, value);
	return true;
}