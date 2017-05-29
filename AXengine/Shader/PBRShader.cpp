#include "AXengine/Shader/PBRShader.h"

#include "AXengine/Asset/PBRMaterial.h"
#include "AXengine/Entity/Cubemap.h"
#include "AXengine/Entity/EnvironmentProbe.h"
#include "AXengine/Entity/DirectionalLight.h"
#include "AXengine/Entity/PointLight.h"
#include "AXengine/Entity/SpotLight.h"
#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>

namespace AX { namespace Shader {

enum PBRShaderTexture {
	ALBEDO,
	EMISSION,
	NORMAL,
	METALLIC,
	ROUGHNESS,
	AO,

	ENVIRONMENT_BRDF,
	ENVIRONMENT_IRRADIANCE = 16,
	ENVIRONMENT_FILTER = 24
};

void PBRShader::ProcessScene(const Entity::Scene& scene)
{
	// Select active lights
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

	// Save environment probes
	_environmentProbeList.clear();
	_environmentProbeList = scene.GetEnvironmentProbeList();
	
	// Save projection matrix
	_projectionMatrix = scene.projectionMatrix;

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
		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_color, directionalLight->diffuseIntensity);
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_directionalLight_color, glm::vec3(-1, -1, -1));
	}
	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		if(pointLight[i])
		{
			glm::vec3 lightPositionOnCamera = glm::vec3(_viewMatrix * glm::vec4(pointLight[i]->position, 1.0f));

			ShaderProgram::LoadUniform(_uniform_fs_pointLight_position[i], lightPositionOnCamera);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_color[i], pointLight[i]->diffuseIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_constant[i], pointLight[i]->constant);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_linear[i], pointLight[i]->linear);
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_quadric[i], pointLight[i]->quadric);
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_pointLight_color[i], glm::vec3(-1, -1, -1));
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
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_color[i], spotLight[i]->diffuseIntensity);
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_innerCutoff[i], glm::cos(glm::radians(spotLight[i]->cutoff)));
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_outerCutoff[i], glm::cos(glm::radians(spotLight[i]->cutoff * spotLight[i]->outerCutoffFactor)));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_spotLight_color[i], glm::vec3(-1, -1, -1));
		}
	}
}
void PBRShader::ProcessMaterial(const Asset::Material& material)
{
	if(isDebugMode)
	{
		// Give black diffuse in debug mode
		ShaderProgram::LoadUniform(_uniform_fs_material_albedoMap_value, glm::vec4(0, 0, 0, 1));
		ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, glm::vec3(0, 0, 0));
		ShaderProgram::LoadUniform(_uniform_fs_material_metallicMap_value, 0.0f);
		ShaderProgram::LoadUniform(_uniform_fs_material_roughnessMap_value, 1.0f);
		ShaderProgram::LoadUniform(_uniform_fs_material_aoMap_value, 1.0f);
	}
	else
	{
		// Process material
		const Asset::PBRMaterial* pbrMaterial = static_cast<const Asset::PBRMaterial*>(&material);

		// Diffuse(Albedo) Map
		if(pbrMaterial->diffuseMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::ALBEDO);
			glBindTexture(GL_TEXTURE_2D, pbrMaterial->diffuseMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_albedoMap_value, glm::vec4(-1, -1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_albedoMap_value, pbrMaterial->diffuseMap.value);
		}

		// Emission Map
		if(pbrMaterial->emissionMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::EMISSION);
			glBindTexture(GL_TEXTURE_2D, pbrMaterial->emissionMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, pbrMaterial->emissionMap.value);
		}

		// Normal Map
		if(pbrMaterial->normalMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::NORMAL);
			glBindTexture(GL_TEXTURE_2D, pbrMaterial->normalMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_normalMap_value, glm::vec3(-1, -1, -1));
		}
		else if(pbrMaterial->normalMap.value.r == -1)
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_normalMap_value, glm::vec3(-2, -2, -2));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_normalMap_value, pbrMaterial->normalMap.value);
		}

		// Metallic Map
		if(pbrMaterial->metallicMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::METALLIC);
			glBindTexture(GL_TEXTURE_2D, pbrMaterial->metallicMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_metallicMap_value, -1.0f);
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_metallicMap_value, pbrMaterial->metallicMap.value);
		}

		// Roughness Map
		if(pbrMaterial->roughnessMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::ROUGHNESS);
			glBindTexture(GL_TEXTURE_2D, pbrMaterial->roughnessMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_roughnessMap_value, -1.0f);
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_roughnessMap_value, pbrMaterial->roughnessMap.value);
		}

		// AO Map
		if(pbrMaterial->aoMap.texture)
		{
			glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::AO);
			glBindTexture(GL_TEXTURE_2D, pbrMaterial->aoMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_aoMap_value, -1.0f);
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_aoMap_value, pbrMaterial->aoMap.value);
		}
	}
}
void PBRShader::ProcessTransform(const Entity::Transform& transform)
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

	// Select closest environment maps
	std::vector<std::pair<Entity::Cubemap, Tool::F64>> activeEnvironmentList;
	std::map<Tool::F64, const Entity::EnvironmentProbe*> environmentDistanceMap;
	for( auto& environmentProbe : _environmentProbeList )
	{
		Tool::F64 distance = glm::distance(transform.position, environmentProbe->GetPositon());
		while(environmentDistanceMap.find(distance) != environmentDistanceMap.end())
			distance += 0.0001;
		environmentDistanceMap[distance] = environmentProbe;
	}
	for( auto& dist_env : environmentDistanceMap )
	{
		if(dist_env.first>=Entity::EnvironmentProbe::GetMaxEffectVolume() || activeEnvironmentList.size()>ENVIRONMENT_COUNT)
			break;
		else if(dist_env.second != nullptr && dist_env.first < dist_env.second->GetEffectVolume())
			activeEnvironmentList.push_back(std::pair<Entity::Cubemap, Tool::F64>(dist_env.second->GetEnvironmentMap(), dist_env.first));
	}

	// Convert distances to weights
	Tool::F64 redundantWeight = 0.0;
	for( auto& env_dist : activeEnvironmentList )
	{
		Tool::F64 effectVolume = environmentDistanceMap[env_dist.second]->GetEffectVolume();

		// TODO: Decide between distance functions

		// Distance Function 1
		// Blends in/out fast, strong influence in close range
		env_dist.second =  1.0 - glm::pow(env_dist.second/effectVolume, 2);

		// Distance Function 2
		// Blends in/out smooth, weaker influence in close range
		/*
		Tool::F64 scaler = (2.0 * env_dist.second)/effectVolume - 1.0;
		scaler = scaler > 0 ? glm::pow(scaler, 3.0/5.0) : -glm::pow(glm::abs(scaler), 3.0/5.0);
		env_dist.second =  (1.0 - scaler) / 2.0;
		*/

		redundantWeight += env_dist.second;
	}
	if(redundantWeight > 0)
		redundantWeight = 0.0f;
	else
		redundantWeight = (1.0 - redundantWeight) / static_cast<Tool::F64>(activeEnvironmentList.size());
	
	// Load environment maps
	for( Tool::SIZE i=0 ; i<ENVIRONMENT_COUNT ; i++ )
	{
		if(activeEnvironmentList.size() > i)
		{
			auto& env_dist = activeEnvironmentList[i];
			loadEnvironment(i, &env_dist.first, env_dist.second+redundantWeight);
		}
		else
		{
			loadEnvironment(i, nullptr, 0);
		}
	}
}

/***** PROTECTED *****/

bool PBRShader::Init()
{
	bool initResult = ShaderProgram::Init("Shader/PBRVertex.glsl", "Shader/PBRFragment.glsl");
	if(initResult == false)
		return false;

	// Set uniform texture positions
	Start();
	
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.albedoMap.texture"), PBRShaderTexture::ALBEDO);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.emissionMap.texture"), PBRShaderTexture::EMISSION);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.normalMap.texture"), PBRShaderTexture::NORMAL);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.metallicMap.texture"), PBRShaderTexture::METALLIC);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.roughnessMap.texture"), PBRShaderTexture::ROUGHNESS);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.aoMap.texture"), PBRShaderTexture::AO);

	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_environmentBRDFLUT"), PBRShaderTexture::ENVIRONMENT_BRDF);
	for( int i=0 ; i<ENVIRONMENT_COUNT ; i++ )
	{
		std::string uniformNameList[] = {
			"fs_environmentMap[i].irradianceMap",
			"fs_environmentMap[i].filterMap"
		};

		for( int j=0 ; j<sizeof(uniformNameList)/sizeof(std::string) ; j++ )
			uniformNameList[j][18] = '0' + i;

		int j = 0;
		ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str()), PBRShaderTexture::ENVIRONMENT_IRRADIANCE + i);
		ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str()), PBRShaderTexture::ENVIRONMENT_FILTER + i);
	}
	
	Stop();

	return true;
}

void PBRShader::BindShaderAttributes()
{
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::POSITION, "attrib_position");
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::UVCOORD, "attrib_uvCoord");
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::NORMAL, "attrib_normal");
	ShaderProgram::BindAttribute(Asset::Mesh::VBOlayout::TANGENT, "attrib_tangent");
}
void PBRShader::GetShaderUniformLocations()
{
	_uniform_vs_ModelViewProjectionMatrix = ShaderProgram::GetUniformLocation("vs_ModelViewProjectionMatrix");
	_uniform_vs_modelViewMatrix = ShaderProgram::GetUniformLocation("vs_modelViewMatrix");
	_uniform_vs_modelMatrix = ShaderProgram::GetUniformLocation("vs_modelMatrix");
	_uniform_vs_normalMatrix = ShaderProgram::GetUniformLocation("vs_normalMatrix");

	_uniform_fs_cameraPosition = ShaderProgram::GetUniformLocation("fs_cameraPosition");

	_uniform_fs_material_albedoMap_value = ShaderProgram::GetUniformLocation("fs_material.albedoMap.value");
	_uniform_fs_material_emissionMap_value = ShaderProgram::GetUniformLocation("fs_material.emissionMap.value");
	_uniform_fs_material_normalMap_value = ShaderProgram::GetUniformLocation("fs_material.normalMap.value");
	_uniform_fs_material_metallicMap_value = ShaderProgram::GetUniformLocation("fs_material.metallicMap.value");
	_uniform_fs_material_roughnessMap_value = ShaderProgram::GetUniformLocation("fs_material.roughnessMap.value");
	_uniform_fs_material_aoMap_value = ShaderProgram::GetUniformLocation("fs_material.aoMap.value");
	
	for( int i=0 ; i<ENVIRONMENT_COUNT ; i++ )
	{
		std::string uniformNameList[] = {
			"fs_environmentMap[i].filterMaxLOD",
			"fs_environmentMap[i].weight"
		};

		for( int j=0 ; j<sizeof(uniformNameList)/sizeof(std::string) ; j++ )
			uniformNameList[j][18] = '0' + i;

		int j = 0;
		_uniform_fs_environmentMap_filterMaxLOD[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_environmentMap_weight[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
	}

	_uniform_fs_directionalLight_direction = ShaderProgram::GetUniformLocation("fs_directionalLight.direction");
	_uniform_fs_directionalLight_color = ShaderProgram::GetUniformLocation("fs_directionalLight.color");

	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		std::string uniformNameList[] = {
			"fs_pointLight[i].position",
			"fs_pointLight[i].color",
			"fs_pointLight[i].constant",
			"fs_pointLight[i].linear",
			"fs_pointLight[i].quadric"
		};

		for( int j=0 ; j<sizeof(uniformNameList)/sizeof(std::string) ; j++ )
			uniformNameList[j][14] = '0' + i;

		int j = 0;
		_uniform_fs_pointLight_position[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_pointLight_color[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_pointLight_constant[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_pointLight_linear[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_pointLight_quadric[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
	}

	for( int i=0 ; i<SPOT_LIGHT_COUNT ; i++ )
	{
		std::string uniformNameList[] = {
			"fs_spotLight[i].position",
			"fs_spotLight[i].direction",
			"fs_spotLight[i].color",
			"fs_spotLight[i].innerCutoff",
			"fs_spotLight[i].outerCutoff"
		};

		for( int j=0 ; j<sizeof(uniformNameList)/sizeof(std::string) ; j++ )
			uniformNameList[j][13] = '0' + i;

		int j = 0;
		_uniform_fs_spotLight_position[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_spotLight_direction[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_spotLight_color[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_spotLight_innerCutoff[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
		_uniform_fs_spotLight_outerCutoff[i] = ShaderProgram::GetUniformLocation(uniformNameList[j++].c_str());
	}
}

/***** PRIVATE *****/

void PBRShader::loadEnvironment(Tool::SIZE index, const Entity::Cubemap* environmentMap, Tool::F32 weight)
{
	if(environmentMap)
	{
		glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::ENVIRONMENT_IRRADIANCE+index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMap->material.reflectionMap.texture->GetTextureID());

		Entity::Cubemap* specularEnvironment = environmentMap->material.environmentMap;
		glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::ENVIRONMENT_FILTER+index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, specularEnvironment->material.diffuseMap.texture->GetTextureID());

		ShaderProgram::LoadUniform(_uniform_fs_environmentMap_filterMaxLOD[index], specularEnvironment->material.diffuseMap.value.r);
		ShaderProgram::LoadUniform(_uniform_fs_environmentMap_weight[index], weight);

		glActiveTexture(GL_TEXTURE0 + PBRShaderTexture::ENVIRONMENT_BRDF);
		glBindTexture(GL_TEXTURE_2D, specularEnvironment->material.reflectionMap.texture->GetTextureID());
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_environmentMap_weight[index], 0.0f);
	}
}

} } // namespace AX::Shader
