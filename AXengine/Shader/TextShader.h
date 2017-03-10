/**
 *	File: AXengine/Shader/TextShader.h
 *	Purpose: Shader program for text rendering
 */

#ifndef __AX__SHADER__TEXT_SHADER_H
#define __AX__SHADER__TEXT_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class TextShader : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override { }
	virtual void ProcessMaterial(const Asset::Material& material) override;
	virtual void ProcessTransform(const Entity::Transform& transform) override { }

protected:
	/**
	 * Init and Terminate is only visible for Engine
	 * 
	 * This is done to prevent API users to call these methods.
	 */
	friend class Core::Engine;
	bool Init(const glm::mat4& projectionMatrix)
	{
		bool initResult = ShaderProgram::Init("Shader/TextVertex.glsl", "Shader/TextFragment.glsl");
		if(initResult == false)
			return false;

		ShaderProgram::Start();
		ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("vs_projectionMatrix"), projectionMatrix);
		ShaderProgram::Stop();
		
		return true;
	}
	void Terminate()
	{
		ShaderProgram::Terminate();
	}

	// Called by base class
	virtual void BindShaderAttributes() override
	{
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::POSITION, "attrib_position");
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_vs_fs_textColor = ShaderProgram::GetUniformLocation("vs_fs_textColor");
	}

private:
	Tool::U32 _uniform_vs_fs_textColor;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__TEXT_SHADER_H
