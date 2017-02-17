#include "AXengine/Shader/StandardShader.h"

#include "AXengine/Model/Material.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void StandardShader::ProcessGameObject(const Entity::GameObject& gameObject, const Entity::Camera*& camera)
{
	// Process material
	if(gameObject.material->diffuseMap.texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameObject.material->diffuseMap.texture->GetTextureID());
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec3(-1, -1, -1));
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, gameObject.material->diffuseMap.value);
	}

	// Process transform
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, gameObject.transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, gameObject.transform.scale);

	// Process camera
	glm::mat4 viewMatrix;
	if(camera)
	{
		viewMatrix = glm::translate(viewMatrix, -camera->transform.position);
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camera->transform.rotation.p), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camera->transform.rotation.y), glm::vec3(0, 1, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camera->transform.rotation.r), glm::vec3(0, 0, 1));
		viewMatrix = glm::scale(viewMatrix, glm::vec3(1.0f, 1.0f, 1.0f)/camera->transform.scale);
	}

	ShaderProgram::LoadUniform(_uniform_vs_ModelViewProjectionMatrix, _projectionMatrix * viewMatrix * modelMatrix);
}

} } // namespace AX::Shader
