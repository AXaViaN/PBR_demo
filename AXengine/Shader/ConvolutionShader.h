/**
 *	File: AXengine/Shader/ConvolutionShader.h
 *	Purpose: Shader program for convoluting cubemaps
 */

#ifndef __AX__SHADER__CONVOLUTION_SHADER_H
#define __AX__SHADER__CONVOLUTION_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Entity {
class EnvironmentProbe;
} }

namespace AX { namespace Shader {

class ConvolutionShader : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override;
	virtual void ProcessMaterial(const Asset::Material& material) override;
	virtual void ProcessTransform(const Entity::Transform& transform) override { }

protected:
	/**
	 * Init and Terminate is only visible for EnvironmentProbe
	 * 
	 * This is done to prevent API users to call these methods.
	 */
	friend class Entity::EnvironmentProbe;
	bool Init(Tool::F32 sampleDelta)
	{
		bool initResult = ShaderProgram::Init("Shader/ConvolutionVertex.glsl", "Shader/ConvolutionFragment.glsl");
		if(initResult == false)
			return false;

		Start();

		ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_sampleDelta"), sampleDelta);

		Stop();

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
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_vs_ViewProjectionMatrix = ShaderProgram::GetUniformLocation("vs_ViewProjectionMatrix");
	}

private:
	glm::mat4 _projectionMatrix;

	Tool::U32 _uniform_vs_ViewProjectionMatrix;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__CONVOLUTION_SHADER_H
