#include "AXengine/Shader/StandardShader.h"

#include "AXengine/Asset/Material.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void StandardShader::ProcessScene(const Entity::Scene& scene)
{
	// Process camera
	if(scene.camera)
	{
		glm::vec3 center = scene.camera->transform.position + scene.camera->GetForwardDirection();
		_viewMatrix = glm::lookAt(scene.camera->transform.position, center, scene.camera->GetUpDirection());
	}
	else
	{
		_viewMatrix = glm::mat4();
	}
}
void StandardShader::ProcessGameObject(const Entity::GameObject& gameObject)
{
	if(isDebugMode)
	{
		// Give green diffuse in debug mode
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec4(0, 1, 0, 1));
	}
	else
	{
		// Process material
		if(gameObject.material->diffuseMap.texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gameObject.material->diffuseMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec4(-1, -1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, gameObject.material->diffuseMap.value);
		}
	}

	// Process transform
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, gameObject.transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, gameObject.transform.scale);

	ShaderProgram::LoadUniform(_uniform_vs_ModelViewProjectionMatrix, _projectionMatrix * _viewMatrix * modelMatrix);
}

} } // namespace AX::Shader
