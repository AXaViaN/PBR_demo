#include "AXengine/Shader/ToneShader.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Asset/Material.h"
#include "AXengine/Core/Engine.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void ToneShader::SetGamma(Tool::F32 gamma)
{
	ToneShader& shader = Core::Engine::Instance().toneShader;
	shader.Start();

	shader.ShaderProgram::LoadUniform(shader._uniform_fs_gamma, gamma);

	shader.Stop();
}
void ToneShader::SetHDRexposure(Tool::F32 exposure)
{
	ToneShader& shader = Core::Engine::Instance().toneShader;
	shader.Start();

	shader.ShaderProgram::LoadUniform(shader._uniform_fs_hdrExposure, exposure);

	shader.Stop();
}

void ToneShader::ProcessMaterial(const Asset::Material& material)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
}

} } // namespace AX::Shader
