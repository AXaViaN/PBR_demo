/**
 *	File: AXengine/Shader/StandardShader.h
 *	Purpose: Shader program for game objects
 */

#ifndef __AX__SHADER__STANDARD_SHADER_H
#define __AX__SHADER__STANDARD_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Shader {

class StandardShader : public ShaderProgram {
public:
	bool Init()
	{
		return ShaderProgram::Init("Shader/StandardVertex.glsl", "Shader/StandardFragment.glsl");
	}
	void Terminate()
	{
		ShaderProgram::Terminate();
	}

	virtual void ProcessMaterial(Model::Material& material) override;

protected:
	// Called by base class
	virtual void BindShaderAttributes() override
	{
		ShaderProgram::BindAttribute(Tool::VBOlayout::POSITION, "vs_position");
		ShaderProgram::BindAttribute(Tool::VBOlayout::UVCOORD, "vs_uvCoord");
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_fs_diffuseValue = ShaderProgram::GetUniformLocation("fs_diffuseValue");
	}

private:
	Tool::U32 _uniform_fs_diffuseValue;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__STANDARD_SHADER_H
