/**
 *	File: AXengine/Shader/GammaShader.h
 *	Purpose: Shader program for gamma correct rendering
 */

#ifndef __AX__SHADER__GAMMA_SHADER_H
#define __AX__SHADER__GAMMA_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class GammaShader : public ShaderProgram {
public:
	static void SetGamma(Tool::F32 gamma);

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
		bool initResult = ShaderProgram::Init("Shader/GammaVertex.glsl", "Shader/GammaFragment.glsl");
		if(initResult == false)
			return false;

		return true;
	}
	void Terminate()
	{
		ShaderProgram::Terminate();
	}

	// Called by base class
	GammaShader() { }
	virtual void BindShaderAttributes() override
	{
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::POSITION, "attrib_position");
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_fs_gamma = ShaderProgram::GetUniformLocation("fs_gamma");
	}

private:
	Tool::U32 _uniform_fs_gamma;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__GAMMA_SHADER_H
