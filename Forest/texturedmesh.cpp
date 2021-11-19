#include "texturedmesh.h"

void TexturedMesh::SetPosition(const glm::vec3& pos)
{
	position = glm::vec3(pos);
}

void TexturedMesh::SetRotation(const glm::vec3& rot)
{
	rotation = glm::vec3(rot);
}

void TexturedMesh::SetScale(const glm::vec3& scale)
{
	this->scale = glm::vec3(scale);
}

glm::mat4 TexturedMesh::M() const
{

	glm::mat4 scaleMatrix = glm::scale(scale);
	
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0, 0, 1));

	glm::mat4 translationMatrix = glm::translate(position);

	return translationMatrix * rotationMatrix * scaleMatrix;
}

TexturedMesh::TexturedMesh() : position(0.0f), rotation(0.0f), scale(1.0f), opacity(1.0f) {}

void TexturedMesh::Draw(const Program& program, const glm::mat4& VP)
{
	glm::mat4 MVP = VP * M();
	program.setUniformMat4("MVP", MVP);
	program.setUniformMat4("M", M());
	program.setUniformMat4("Minv", glm::inverse(M()));
	program.setUniformFloat("opacity", opacity);
}

bool TexturedMesh::isDestroyed()
{
	return destroyed;
}

void TexturedMesh::Destroy()
{
	destroyed = true;
}