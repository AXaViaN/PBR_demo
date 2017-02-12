/**
 *	File: AXengine/Shader/StandardShader.h
 *	Purpose: Shader program for game objects
 */

#ifndef __AX__SHADER__STANDARD_SHADER_H
#define __AX__SHADER__STANDARD_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"

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

protected:
	// Called by base class
	virtual void BindShaderAttributes() override
	{
		ShaderProgram::BindAttribute(Tool::VBOlayout::POSITION, "vs_position");
	}

};

} } // namespace AX::Shader

#endif // __AX__SHADER__STANDARD_SHADER_H
