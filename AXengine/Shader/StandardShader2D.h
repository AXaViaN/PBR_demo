/**
 *	File: AXengine/Shader/StandardShader2D.h
 *	Purpose: Default shader program for 2D rendering
 */

#ifndef __AX__SHADER__STANDARD_SHADER2D_H
#define __AX__SHADER__STANDARD_SHADER2D_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class StandardShader2D : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override { }
	virtual void ProcessMaterial(const Asset::Material& material) override;
	virtual void ProcessTransform(const Entity::Transform& transform) override;

protected:
	/**
	 * Init and Terminate is only visible for Engine
	 * 
	 * This is done to prevent API users to call these methods.
	 */
	friend class Core::Engine;
	bool Init()
	{
		bool initResult = ShaderProgram::Init("Shader/Standard2DVertex.glsl", "Shader/Standard2DFragment.glsl");
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
		_uniform_vs_modelMatrix = ShaderProgram::GetUniformLocation("vs_modelMatrix");

		_uniform_fs_diffuseValue = ShaderProgram::GetUniformLocation("fs_diffuseValue");
	}

private:
	Tool::U32 _uniform_vs_modelMatrix;

	Tool::U32 _uniform_fs_diffuseValue;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__STANDARD_SHADER2D_H
