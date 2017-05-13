/**
 *	File: AXengine/Shader/BRDFIntegrationShader.h
 *	Purpose: Shader program for creating BRDF LUT texture
 */

#ifndef __AX__SHADER__BRDF_INTEGRATION_SHADER_H
#define __AX__SHADER__BRDF_INTEGRATION_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Entity {
class EnvironmentProbe;
} }

namespace AX { namespace Shader {

class BRDFIntegrationShader : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override { }
	virtual void ProcessMaterial(const Asset::Material& material) override { }
	virtual void ProcessTransform(const Entity::Transform& transform) override { }

protected:
	/**
	 * Init and Terminate is only visible for EnvironmentProbe
	 * 
	 * This is done to prevent API users to call these methods.
	 */
	friend class Entity::EnvironmentProbe;
	bool Init(Tool::U32 sampleCount)
	{
		bool initResult = ShaderProgram::Init("Shader/BRDFIntegrationVertex.glsl", "Shader/BRDFIntegrationFragment.glsl");
		if(initResult == false)
			return false;

		Start();

		ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_sampleCount"), sampleCount);

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
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	}

	virtual void GetShaderUniformLocations() override { }

};

} } // namespace AX::Shader

#endif // __AX__SHADER__BRDF_INTEGRATION_SHADER_H
