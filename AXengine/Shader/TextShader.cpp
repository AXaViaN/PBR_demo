#include "AXengine/Shader/TextShader.h"

#include "AXengine/Asset/Material.h"
#include <GL/glew.h>

namespace AX { namespace Shader {

void TextShader::ProcessMaterial(const Asset::Material& material)
{
	// Process material
	if(material.diffuseMap.texture)
	{
		// Render text
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.diffuseMap.texture->GetTextureID());
		ShaderProgram::LoadUniform(_uniform_vs_fs_textColor, glm::vec4(glm::vec3(material.diffuseMap.value), -1));
	}
	else
	{
		// Render background
		ShaderProgram::LoadUniform(_uniform_vs_fs_textColor, material.diffuseMap.value);
	}
}

} } // namespace AX::Shader
