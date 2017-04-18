/**
 *	File: AXengine/Shader/PBRShader.h
 *	Purpose: Shader for PBR lighting
 */

#ifndef __AX__SHADER__PBR_SHADER_H
#define __AX__SHADER__PBR_SHADER_H

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Shader {

class PBRShader : public ShaderProgram {
public:
	virtual void ProcessScene(const Entity::Scene& scene) override;
	virtual void ProcessMaterial(const Asset::Material& material) override;
	virtual void ProcessTransform(const Entity::Transform& transform) override;

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
	Tool::U32 _uniform_vs_modelMatrix;
	Tool::U32 _uniform_vs_normalMatrix;

	Tool::U32 _uniform_fs_material_albedoMap_value;
	Tool::U32 _uniform_fs_material_normalMap_value;
	Tool::U32 _uniform_fs_material_metallicMap_value;
	Tool::U32 _uniform_fs_material_roughnessMap_value;
	Tool::U32 _uniform_fs_material_aoMap_value;

	Tool::U32 _uniform_fs_directionalLight_direction;
	Tool::U32 _uniform_fs_directionalLight_color;

	static const Tool::SIZE POINT_LIGHT_COUNT = 8;
	Tool::U32 _uniform_fs_pointLight_position[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_color[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_constant[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_linear[POINT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_pointLight_quadric[POINT_LIGHT_COUNT];

	static const Tool::SIZE SPOT_LIGHT_COUNT = 8;
	Tool::U32 _uniform_fs_spotLight_position[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_direction[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_color[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_innerCutoff[SPOT_LIGHT_COUNT];
	Tool::U32 _uniform_fs_spotLight_outerCutoff[SPOT_LIGHT_COUNT];

};

} } // namespace AX::Shader

#endif // __AX__SHADER__PBR_SHADER_H
