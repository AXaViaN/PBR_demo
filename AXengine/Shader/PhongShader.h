/**
 *	File: AXengine/Shader/PhongShader.h
 *	Purpose: Shader for basic lighting
 */

#ifndef __AX__SHADER__PHONG_SHADER_H
#define __AX__SHADER__PHONG_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class PhongShader : public ShaderProgram {
public:
	virtual void ProcessGameObject(const Entity::GameObject& gameObject, const Entity::Camera*& camera, const Entity::Light*& light) override;

protected:
	/**
	 * Init and Terminate is only visible for Engine
	 */
	friend class Core::Engine;
	bool Init(const glm::mat4& projectionMatrix);
	void Terminate()
	{
		ShaderProgram::Terminate();
	}

	// Called by base class
	virtual void BindShaderAttributes() override;
	virtual void GetShaderUniformLocations() override;

private:
	glm::mat4 _projectionMatrix;
	Tool::U32 _uniform_vs_ModelViewProjectionMatrix;
	Tool::U32 _uniform_vs_modelViewMatrix;
	Tool::U32 _uniform_vs_normalMatrix;
	Tool::U32 _uniform_vs_lightPositionOnCamera;

	Tool::U32 _uniform_fs_diffuseValue;
	Tool::U32 _uniform_fs_lightColor;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__PHONG_SHADER_H
