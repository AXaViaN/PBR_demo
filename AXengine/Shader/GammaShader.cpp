#include "AXengine/Shader/GammaShader.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Asset/Material.h"
#include "AXengine/Core/Engine.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void GammaShader::SetGamma(Tool::F32 gamma)
{
	GammaShader& shader = Core::Engine::Instance().gammaShader;
	shader.Start();

	shader.ShaderProgram::LoadUniform(shader._uniform_fs_gamma, gamma);

	shader.Stop();
}

void GammaShader::ProcessMaterial(const Asset::Material& material)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
}

} } // namespace AX::Shader
