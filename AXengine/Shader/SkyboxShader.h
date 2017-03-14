/**
 *	File: AXengine/Shader/SkyboxShader.h
 *	Purpose: Default shader program for game objects
 */

#ifndef __AX__SHADER__SKYBOX_SHADER_H
#define __AX__SHADER__SKYBOX_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class SkyboxShader : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override;
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
		bool initResult = ShaderProgram::Init("Shader/SkyboxVertex.glsl", "Shader/SkyboxFragment.glsl");
		if(initResult == false)
			return false;

		_projectionMatrix = projectionMatrix;
		
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

#endif // __AX__SHADER__SKYBOX_SHADER_H
