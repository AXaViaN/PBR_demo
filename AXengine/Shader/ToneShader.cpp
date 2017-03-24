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

	shader._useAutoExposure = false;
}

void ToneShader::ProcessMaterial(const Asset::Material& material)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
}

/***** PROTECTED *****/

bool ToneShader::SetAutoExposure(Tool::F32 exposure)
{
	if(_useAutoExposure)
	{
		SetHDRexposure(exposure);
		// It is set to false in SetHDRexposure()
		_useAutoExposure = true;
	}

	return _useAutoExposure;
}

} } // namespace AX::Shader
