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
	virtual void ProcessScene(const Entity::Scene& scene) override;
	virtual void ProcessGameObject(const Entity::GameObject& gameObject) override;

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
	glm::mat4 _viewMatrix;

	Tool::U32 _uniform_vs_ModelViewProjectionMatrix;
	Tool::U32 _uniform_vs_modelViewMatrix;
	Tool::U32 _uniform_vs_normalMatrix;

	Tool::U32 _uniform_fs_material_diffuseMap_value;
	Tool::U32 _uniform_fs_material_specularMap_value;
	Tool::U32 _uniform_fs_material_emissionMap_value;

	Tool::U32 _uniform_fs_directionalLight_direction;
	Tool::U32 _uniform_fs_directionalLight_color_diffuse;
	Tool::U32 _uniform_fs_directionalLight_color_specular;
	Tool::U32 _uniform_fs_directionalLight_color_ambient;
	
	static const Tool::SIZE POINT_LIGHT_COUNT = 8;
	Tool::U32 _uniform_fs_pointLight_position[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_color_diffuse[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_color_specular[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_color_ambient[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_constant[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_linear[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_quadric[POINT_LIGHT_COUNT];

	static const Tool::SIZE SPOT_LIGHT_COUNT = 8;
	Tool::U32 _uniform_fs_spotLight_position[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_direction[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_color_diffuse[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_color_specular[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_color_ambient[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_innerCutoff[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_outerCutoff[SPOT_LIGHT_COUNT];

};

} } // namespace AX::Shader

#endif // __AX__SHADER__PHONG_SHADER_H
