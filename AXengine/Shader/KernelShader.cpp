#include "AXengine/Shader/KernelShader.h"

#include "AXengine/Asset/Material.h"
#include "AXengine/Core/Engine.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

KernelShader* KernelShader::SetKernel(Tool::F32 kernel[9], Tool::F32 offset)
{
	KernelShader& shader = Core::Engine::Instance().kernelShader;
	shader.Start();

	for( Tool::SIZE i=0 ; i<9 ; i++ )
		shader.ShaderProgram::LoadUniform(shader._uniform_fs_kernel[i], kernel[i]);

	shader.ShaderProgram::LoadUniform(shader._uniform_fs_offset, offset);

	shader.Stop();
	return &shader;
}

void KernelShader::ProcessMaterial(const Asset::Material& material)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
}
void KernelShader::ProcessTransform(const Entity::Transform& transform)
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
