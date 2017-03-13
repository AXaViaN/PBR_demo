#include "AXengine/Shader/StandardShader2D.h"

#include "AXengine/Asset/Material.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void StandardShader2D::ProcessMaterial(const Asset::Material& material)
{
	// Process material
	if(material.diffuseMap.texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec4(-1, -1, -1, -1));
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, material.diffuseMap.value);
	}
}
void StandardShader2D::ProcessTransform(const Entity::Transform& transform)
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, transform.scale);

	ShaderProgram::LoadUniform(_uniform_vs_modelMatrix, modelMatrix);
}

} } // namespace AX::Shader
