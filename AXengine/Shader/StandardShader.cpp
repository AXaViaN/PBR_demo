#include "AXengine/Shader/StandardShader.h"

#include "AXengine/Asset/Material.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void StandardShader::ProcessScene(const Entity::Scene& scene)
{
	_projectionMatrix = scene.projectionMatrix;

	// Process camera
	_viewMatrix = glm::mat4();
	if(scene.camera)
	{
		_viewMatrix = glm::scale(_viewMatrix, glm::vec3(1, 1, 1)/scene.camera->transform.scale);
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(scene.camera->transform.rotation.x), glm::vec3(1, 0, 0));
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(scene.camera->transform.rotation.y), glm::vec3(0, 1, 0));
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(scene.camera->transform.rotation.z), glm::vec3(0, 0, -1));
		_viewMatrix = glm::translate(_viewMatrix, -scene.camera->transform.position);
	}
}
void StandardShader::ProcessMaterial(const Asset::Material& material)
{
	if(isDebugMode)
	{
		// Give green diffuse in debug mode
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec4(0, 1, 0, 1));
	}
	else
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
}
void StandardShader::ProcessTransform(const Entity::Transform& transform)
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, transform.scale);

	ShaderProgram::LoadUniform(_uniform_vs_ModelViewProjectionMatrix, _projectionMatrix * _viewMatrix * modelMatrix);
}

} } // namespace AX::Shader
