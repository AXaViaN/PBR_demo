/**
 *	File: AXengine/Shader/EquirectangularShader.h
 *	Purpose: Shader program for equirectangular hdr skybox rendering
 */

#ifndef __AX__SHADER__EQUIRECTANGULAR_SHADER_H
#define __AX__SHADER__EQUIRECTANGULAR_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class EquirectangularShader : public ShaderProgram {
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
	bool Init()
	{
		bool initResult = ShaderProgram::Init("Shader/EquirectangularVertex.glsl", "Shader/EquirectangularFragment.glsl");
		if(initResult == false)
			return false;

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

#endif // __AX__SHADER__EQUIRECTANGULAR_SHADER_H
