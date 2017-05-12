#include "AXengine/Shader/PreFilterShader.h"

#include "AXengine/Asset/Material.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void PreFilterShader::ProcessScene(const Entity::Scene& scene)
{
	_projectionMatrix = scene.projectionMatrix;

	// Process camera
	glm::mat4 viewMatrix;
	if(scene.camera)
	{
		viewMatrix = glm::scale(viewMatrix, glm::vec3(1, 1, 1)/scene.camera->transform.scale);
		viewMatrix = glm::rotate(viewMatrix, glm::radians(scene.camera->transform.rotation.x), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(scene.camera->transform.rotation.y), glm::vec3(0, 1, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(scene.camera->transform.rotation.z), glm::vec3(0, 0, -1));
	}

	ShaderProgram::LoadUniform(_uniform_vs_ViewProjectionMatrix, _projectionMatrix * viewMatrix);
}
void PreFilterShader::ProcessMaterial(const Asset::Material& material)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, material.diffuseMap.texture->GetTextureID());
	
	// Reflection value is processed as roughness value
	ShaderProgram::LoadUniform(_uniform_fs_roughness, material.reflectionMap.value);

	// Diffuse value.r is processed as resolution
	ShaderProgram::LoadUniform(_uniform_fs_resolution, material.diffuseMap.value.r);
}

} } // namespace AX::Shader
