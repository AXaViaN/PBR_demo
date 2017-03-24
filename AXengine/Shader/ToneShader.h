/**
 *	File: AXengine/Shader/ToneShader.h
 *	Purpose: Shader program for fine tuning scene
 */

#ifndef __AX__SHADER__TONE_SHADER_H
#define __AX__SHADER__TONE_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class ToneShader : public ShaderProgram {
public:
	static void SetGamma(Tool::F32 gamma);
	static void SetHDRexposure(Tool::F32 exposure);

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
	bool Init()
	{
		bool initResult = ShaderProgram::Init("Shader/ToneVertex.glsl", "Shader/ToneFragment.glsl");
		if(initResult == false)
			return false;

		_useAutoExposure = true;

		Start();
		ShaderProgram::LoadUniform(_uniform_fs_gamma, 1.0f);
		ShaderProgram::LoadUniform(_uniform_fs_hdrExposure, 1.0f);
		Stop();

		return true;
	}
	void Terminate()
	{
		ShaderProgram::Terminate();
	}
	bool SetAutoExposure(Tool::F32 exposure);

	// Called by base class
	ToneShader() { }
	virtual void BindShaderAttributes() override
	{
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::POSITION, "attrib_position");
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_fs_gamma = ShaderProgram::GetUniformLocation("fs_gamma");
		_uniform_fs_hdrExposure = ShaderProgram::GetUniformLocation("fs_hdrExposure");
	}

private:
	Tool::U32 _uniform_fs_gamma;
	Tool::U32 _uniform_fs_hdrExposure;

	bool _useAutoExposure;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__TONE_SHADER_H
