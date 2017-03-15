#include "AXengine/Shader/PhongShader.h"

#include "AXengine/Asset/PhongMaterial.h"
#include "AXengine/Entity/Cubemap.h"
#include "AXengine/Entity/DirectionalLight.h"
#include "AXengine/Entity/PointLight.h"
#include "AXengine/Entity/SpotLight.h"
#include "AXengine/Gfx/Renderer.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace AX { namespace Shader {

enum PhongShaderTexture {
	DIFFUSE,
	SPECULAR,
	EMISSION,
	REFLECTION,

	ENVIRONMENT
};

void PhongShader::ProcessScene(const Entity::Scene& scene)
{
	const Entity::DirectionalLight* directionalLight = nullptr;
	const Entity::PointLight* pointLight[POINT_LIGHT_COUNT] = {nullptr};
	const Entity::SpotLight* spotLight[SPOT_LIGHT_COUNT] = {nullptr};
	Tool::SIZE activePointLightCount = 0;
	Tool::SIZE activeSpotLightCount = 0;
	auto& lightList = scene.GetLightList();
	for( int i=0 ; i<lightList.size() ; i++ )
	{
		if(lightList[i]->type == Entity::Light::DIRECTIONAL)
		{
			directionalLight = static_cast<const Entity::DirectionalLight*>(lightList[i]);
		}
		else if(lightList[i]->type == Entity::Light::POINT)
		{
			if(activePointLightCount < POINT_LIGHT_COUNT)
				pointLight[activePointLightCount++] = static_cast<const Entity::PointLight*>(lightList[i]);
		}
		else if(lightList[i]->type == Entity::Light::SPOT)
		{
			if(activeSpotLightCount < POINT_LIGHT_COUNT)
				spotLight[activeSpotLightCount++] = static_cast<const Entity::SpotLight*>(lightList[i]);
		}
	}

	if(isDebugMode)
	{
		directionalLight = nullptr;
		for( int i=0 ; i<activePointLightCount ; i++ )
			pointLight[i] = nullptr;
		activePointLightCount = 0;
		for( int i=0 ; i<activeSpotLightCount ; i++ )
			spotLight[i] = nullptr;
		activeSpotLightCount = 0;
	}

	// Process camera
	_viewMatrix = glm::mat4();
	if(scene.camera)
	{
		_viewMatrix = glm::scale(_viewMatrix, glm::vec3(1, 1, 1)/scene.camera->transform.scale);
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(scene.camera->transform.rotation.x), glm::vec3(1, 0, 0));
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(scene.camera->transform.rotation.y), glm::vec3(0, 1, 0));
		_viewMatrix = glm::rotate(_viewMatrix, glm::radians(scene.camera->transform.rotation.z), glm::vec3(0, 0, -1));
		_viewMatrix = glm::translate(_viewMatrix, -scene.camera->transform.position);

		ShaderProgram::LoadUniform(_uniform_fs_cameraPosition, scene.camera->transform.position);
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_cameraPosition, glm::vec3());
	}

	// Process lights
	if(directionalLight)
	{
		glm::vec3 lightDirectionOnCamera = glm::vec3(_viewMatrix * glm::vec4(directionalLight->direction, 0.0f));

		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_direction, lightDirectionOnCamera);
		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_color_diffuse, directionalLight->diffuseIntensity);
		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_color_specular, directionalLight->specularIntensity);
		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_color_ambient, directionalLight->ambientIntensity);
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_color_diffuse, glm::vec3(-1, -1, -1));
	}
	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		if(pointLight[i])
		{
			glm::vec3 lightPositionOnCamera = glm::vec3(_viewMatrix * glm::vec4(pointLight[i]->position, 1.0f));

			ShaderProgram::LoadUniform(_uniform_fs_pointLight_position[i], lightPositionOnCamera);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_color_diffuse[i], pointLight[i]->diffuseIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_color_specular[i], pointLight[i]->specularIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_color_ambient[i], pointLight[i]->ambientIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_constant[i], pointLight[i]->constant);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_linear[i], pointLight[i]->linear);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_quadric[i], pointLight[i]->quadric);
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_color_diffuse[i], glm::vec3(-1, -1, -1));
		}
	}
	for( int i=0 ; i<SPOT_LIGHT_COUNT ; i++ )
	{
		if(spotLight[i])
		{
			glm::vec3 lightPositionOnCamera = glm::vec3(_viewMatrix * glm::vec4(spotLight[i]->position, 1.0f));
			glm::vec3 lightDirectionOnCamera = glm::vec3(_viewMatrix * glm::vec4(spotLight[i]->direction, 0.0f));

			ShaderProgram::LoadUniform(_uniform_fs_spotLight_position[i], lightPositionOnCamera);
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_direction[i], lightDirectionOnCamera);
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_color_diffuse[i], spotLight[i]->diffuseIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_color_specular[i], spotLight[i]->specularIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_color_ambient[i], spotLight[i]->ambientIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_innerCutoff[i], glm::cos(glm::radians(spotLight[i]->cutoff)));
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_outerCutoff[i], glm::cos(glm::radians(spotLight[i]->cutoff * spotLight[i]->outerCutoffFactor)));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_color_diffuse[i], glm::vec3(-1, -1, -1));
		}
	}
}
void PhongShader::ProcessMaterial(const Asset::Material& material)
{
	if(isDebugMode)
	{
		// Give green diffuse in debug mode
		ShaderProgram::LoadUniform(_uniform_fs_material_diffuseMap_value, glm::vec4(0, 1, 0, 1));
		ShaderProgram::LoadUniform(_uniform_fs_material_specularMap_value, glm::vec3(0, 1, 0));
		ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, glm::vec3(0, 1, 0));
		ShaderProgram::LoadUniform(_uniform_fs_material_reflectionMap_value, glm::vec3(0, 0, 0));
		ShaderProgram::LoadUniform(_uniform_fs_material_shininess, 128.0f);
	}
	else
	{
		// Process material
		const Asset::PhongMaterial* phongMaterial = static_cast<const Asset::PhongMaterial*>(&material);

		// Diffuse Map
		if(phongMaterial->diffuseMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PhongShaderTexture::DIFFUSE);
			glBindTexture(GL_TEXTURE_2D, phongMaterial->diffuseMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_diffuseMap_value, glm::vec4(-1, -1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_diffuseMap_value, phongMaterial->diffuseMap.value);
		}

		// Specular Map
		if(phongMaterial->specularMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PhongShaderTexture::SPECULAR);
			glBindTexture(GL_TEXTURE_2D, phongMaterial->specularMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_specularMap_value, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_specularMap_value, phongMaterial->specularMap.value);
		}
		ShaderProgram::LoadUniform(_uniform_fs_material_shininess, phongMaterial->shininess);

		// Emission Map
		if(phongMaterial->emissionMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PhongShaderTexture::EMISSION);
			glBindTexture(GL_TEXTURE_2D, phongMaterial->emissionMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, phongMaterial->emissionMap.value);
		}
		
		// Environment Map && Reflection Map
		// Don't use reflection map if no environment map is avaible
		if(phongMaterial->environmentMap)
		{
			glActiveTexture(GL_TEXTURE0 + PhongShaderTexture::ENVIRONMENT);
			glBindTexture(GL_TEXTURE_CUBE_MAP, phongMaterial->environmentMap->material.diffuseMap.texture->GetTextureID());

			if(phongMaterial->reflectionMap.texture)
			{
				glActiveTexture(GL_TEXTURE0 + PhongShaderTexture::REFLECTION);
				glBindTexture(GL_TEXTURE_2D, phongMaterial->reflectionMap.texture->GetTextureID());
				ShaderProgram::LoadUniform(_uniform_fs_material_reflectionMap_value, -1.0f);
			}
			else
			{
				ShaderProgram::LoadUniform(_uniform_fs_material_reflectionMap_value, phongMaterial->reflectionMap.value);
			}
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_reflectionMap_value, 0.0f);
		}
	}
}
void PhongShader::ProcessTransform(const Entity::Transform& transform)
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, transform.scale);

	glm::mat4 modelViewMatrix = _viewMatrix * modelMatrix;
	ShaderProgram::LoadUniform(_uniform_vs_modelMatrix, modelMatrix);
	ShaderProgram::LoadUniform(_uniform_vs_modelViewMatrix, modelViewMatrix);
	ShaderProgram::LoadUniform(_uniform_vs_ModelViewProjectionMatrix, _projectionMatrix * modelViewMatrix);

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));
	ShaderProgram::LoadUniform(_uniform_vs_normalMatrix, normalMatrix);
}

/***** PROTECTED *****/

bool PhongShader::Init(const glm::mat4& projectionMatrix)
{
	bool initResult = ShaderProgram::Init("Shader/PhongVertex.glsl", "Shader/PhongFragment.glsl");
	if(initResult == false)
		return false;

	_projectionMatrix = projectionMatrix;

	// Set uniform texture positions
	Start();
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_nearPlane"), Gfx::Renderer::NEAR_PLANE);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_farPlane"), Gfx::Renderer::FAR_PLANE);

	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.diffuseMap.texture"), PhongShaderTexture::DIFFUSE);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.specularMap.texture"), PhongShaderTexture::SPECULAR);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.emissionMap.texture"), PhongShaderTexture::EMISSION);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.reflectionMap.texture"), PhongShaderTexture::REFLECTION);

	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_environmentMap"), PhongShaderTexture::ENVIRONMENT);

	Stop();

	return true;
}

void PhongShader::BindShaderAttributes()
{
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::POSITION, "attrib_position");
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::NORMAL, "attrib_normal");
}
void PhongShader::GetShaderUniformLocations()
{
	_uniform_vs_ModelViewProjectionMatrix = ShaderProgram::GetUniformLocation("vs_ModelViewProjectionMatrix");
	_uniform_vs_modelViewMatrix = ShaderProgram::GetUniformLocation("vs_modelViewMatrix");
	_uniform_vs_modelMatrix = ShaderProgram::GetUniformLocation("vs_modelMatrix");
	_uniform_vs_normalMatrix = ShaderProgram::GetUniformLocation("vs_normalMatrix");

	_uniform_fs_cameraPosition = ShaderProgram::GetUniformLocation("fs_cameraPosition");

	_uniform_fs_material_diffuseMap_value = ShaderProgram::GetUniformLocation("fs_material.diffuseMap.value");
	_uniform_fs_material_specularMap_value = ShaderProgram::GetUniformLocation("fs_material.specularMap.value");
	_uniform_fs_material_emissionMap_value = ShaderProgram::GetUniformLocation("fs_material.emissionMap.value");
	_uniform_fs_material_reflectionMap_value = ShaderProgram::GetUniformLocation("fs_material.reflectionMap.value");
	_uniform_fs_material_shininess = ShaderProgram::GetUniformLocation("fs_material.shininess");

	_uniform_fs_directionalLight_direction = ShaderProgram::GetUniformLocation("fs_directionalLight.direction");
	_uniform_fs_directionalLight_color_diffuse = ShaderProgram::GetUniformLocation("fs_directionalLight.color.diffuse");
	_uniform_fs_directionalLight_color_specular = ShaderProgram::GetUniformLocation("fs_directionalLight.color.specular");
	_uniform_fs_directionalLight_color_ambient = ShaderProgram::GetUniformLocation("fs_directionalLight.color.ambient");

	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		std::string uniformNameList[] = {
			"fs_pointLight[i].position",
			"fs_pointLight[i].color.diffuse",
			"fs_pointLight[i].color.specular",
			"fs_pointLight[i].color.ambient",
			"fs_pointLight[i].constant",
			"fs_pointLight[i].linear",
			"fs_pointLight[i].quadric"
		};

		for( int j=0 ; j<sizeof(uniformNameList)/sizeof(std::string) ; j++ )
			uniformNameList[j][14] = '0' + i;

		_uniform_fs_pointLight_position[i] = ShaderProgram::GetUniformLocation(uniformNameList[0].c_str());
		_uniform_fs_pointLight_color_diffuse[i] = ShaderProgram::GetUniformLocation(uniformNameList[1].c_str());
		_uniform_fs_pointLight_color_specular[i] = ShaderProgram::GetUniformLocation(uniformNameList[2].c_str());
		_uniform_fs_pointLight_color_ambient[i] = ShaderProgram::GetUniformLocation(uniformNameList[3].c_str());
		_uniform_fs_pointLight_constant[i] = ShaderProgram::GetUniformLocation(uniformNameList[4].c_str());
		_uniform_fs_pointLight_linear[i] = ShaderProgram::GetUniformLocation(uniformNameList[5].c_str());
		_uniform_fs_pointLight_quadric[i] = ShaderProgram::GetUniformLocation(uniformNameList[6].c_str());
	}

	for( int i=0 ; i<SPOT_LIGHT_COUNT ; i++ )
	{
		std::string uniformNameList[] = {
			"fs_spotLight[i].position",
			"fs_spotLight[i].direction",
			"fs_spotLight[i].color.diffuse",
			"fs_spotLight[i].color.specular",
			"fs_spotLight[i].color.ambient",
			"fs_spotLight[i].innerCutoff",
			"fs_spotLight[i].outerCutoff"
		};

		for( int j=0 ; j<sizeof(uniformNameList)/sizeof(std::string) ; j++ )
			uniformNameList[j][13] = '0' + i;

		_uniform_fs_spotLight_position[i] = ShaderProgram::GetUniformLocation(uniformNameList[0].c_str());
		_uniform_fs_spotLight_direction[i] = ShaderProgram::GetUniformLocation(uniformNameList[1].c_str());
		_uniform_fs_spotLight_color_diffuse[i] = ShaderProgram::GetUniformLocation(uniformNameList[2].c_str());
		_uniform_fs_spotLight_color_specular[i] = ShaderProgram::GetUniformLocation(uniformNameList[3].c_str());
		_uniform_fs_spotLight_color_ambient[i] = ShaderProgram::GetUniformLocation(uniformNameList[4].c_str());
		_uniform_fs_spotLight_innerCutoff[i] = ShaderProgram::GetUniformLocation(uniformNameList[5].c_str());
		_uniform_fs_spotLight_outerCutoff[i] = ShaderProgram::GetUniformLocation(uniformNameList[6].c_str());
	}
}

} } // namespace AX::Shader
