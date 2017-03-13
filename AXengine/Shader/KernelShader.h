/**
 *	File: AXengine/Shader/KernelShader.h
 *	Purpose: Default shader program for 2D rendering
 */

#ifndef __AX__SHADER__KERNEL_SHADER_H
#define __AX__SHADER__KERNEL_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class KernelShader : public ShaderProgram {
public:
	static KernelShader* SetKernel(Tool::F32 kernel[9], Tool::F32 offset);

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
		bool initResult = ShaderProgram::Init("Shader/KernelVertex.glsl", "Shader/KernelFragment.glsl");
		if(initResult == false)
			return false;

		return true;
	}
	void Terminate()
	{
		ShaderProgram::Terminate();
	}

	// Called by base class
	KernelShader() { }
	virtual void BindShaderAttributes() override
	{
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::POSITION, "attrib_position");
		ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	}

	virtual void GetShaderUniformLocations() override
	{
		_uniform_vs_modelMatrix = ShaderProgram::GetUniformLocation("vs_modelMatrix");

		_uniform_fs_offset = ShaderProgram::GetUniformLocation("fs_offset");

		std::string kernelStr = "fs_kernel[i]";
		for( Tool::SIZE i=0 ; i<9 ; i++ )
		{
			kernelStr[10] = '0' + i;
			_uniform_fs_kernel[i] = ShaderProgram::GetUniformLocation(kernelStr.c_str());
		}
	}

private:
	Tool::U32 _uniform_vs_modelMatrix;

	Tool::U32 _uniform_fs_offset;
	Tool::U32 _uniform_fs_kernel[9];

};

} } // namespace AX::Shader

#endif // __AX__SHADER__KERNEL_SHADER_H
