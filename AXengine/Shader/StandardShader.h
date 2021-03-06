/**
 *	File: AXengine/Shader/StandardShader.h
 *	Purpose: Default shader program for game objects
 */

#ifndef __AX__SHADER__STANDARD_SHADER_H
#define __AX__SHADER__STANDARD_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class StandardShader : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override;
	virtual void ProcessMaterial(const Asset::Material& material) override;
	virtual void ProcessTransform(const Entity::Transform& transform) override;

protected:
	/**
	 * Init and Terminate is only visible for Engine
	 * 
	 * This is done to prevent API users to call these methods.
	 * StandardShader is the default shader of the engine.
	 */
	friend class Core::Engine;
	bool Init()
	{
		bool initResult = ShaderProgram::Init("Shader/StandardVertex.glsl", "Shader/StandardFragment.glsl");
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
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_vs_ModelViewProjectionMatrix = ShaderProgram::GetUniformLocation("vs_ModelViewProjectionMatrix");

		_uniform_fs_diffuseValue = ShaderProgram::GetUniformLocation("fs_diffuseValue");
	}

private:
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;

	Tool::U32 _uniform_vs_ModelViewProjectionMatrix;

	Tool::U32 _uniform_fs_diffuseValue;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__STANDARD_SHADER_H
