#include "AXengine/Shader/StandardShader.h"

#include <GL/glew.h>

namespace AX { namespace Shader {

void StandardShader::ProcessMaterial(Model::Material& material)
{
	if(material.diffuseMap.texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec3(-1, -1, -1));
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, material.diffuseMap.value);
	}
}

} } // namespace AX::Shader
