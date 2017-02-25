#include "AXengine/Shader/PhongShader.h"

#include "AXengine/Model/PhongMaterial.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Shader {

void PhongShader::ProcessGameObject(const Entity::GameObject& gameObject, const Entity::Camera*& camera, const Entity::Light*& light)
{
	if(isDebugMode)
	{
		// Give green diffuse in debug mode
		ShaderProgram::LoadUniform(_uniform_fs_material_diffuseMap_value, glm::vec3(0, 1, 0));
		ShaderProgram::LoadUniform(_uniform_fs_material_specularMap_value, glm::vec3(0, 1, 0));
		ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, glm::vec3(0, 1, 0));
		light = nullptr;
	}
	else
	{
		// Process material
		Model::PhongMaterial* material = static_cast<Model::PhongMaterial*>(gameObject.material);
		
		// Diffuse Map
		if(material->diffuseMap.texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuseMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_diffuseMap_value, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_diffuseMap_value, material->diffuseMap.value);
		}
		
		// Specular Map
		if(material->specularMap.texture)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->specularMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_specularMap_value, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_specularMap_value, material->specularMap.value);
		}

		// Emission Map
		if(material->emissionMap.texture)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, material->emissionMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_material_emissionMap_value, material->emissionMap.value);
		}
	}
	
	// Process transform
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, gameObject.transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.x), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.y), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(gameObject.transform.rotation.z), glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, gameObject.transform.scale);

	// Process camera
	glm::mat4 viewMatrix;
	if(camera)
	{
		viewMatrix = glm::scale(viewMatrix, glm::vec3(1, 1, 1)/camera->transform.scale);
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camera->transform.rotation.x), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camera->transform.rotation.y), glm::vec3(0, 1, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camera->transform.rotation.z), glm::vec3(0, 0, -1));
		viewMatrix = glm::translate(viewMatrix, -camera->transform.position);
	}
	
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	ShaderProgram::LoadUniform(_uniform_vs_modelViewMatrix, modelViewMatrix);
	ShaderProgram::LoadUniform(_uniform_vs_ModelViewProjectionMatrix, _projectionMatrix * modelViewMatrix);

	// Process light
	if(light)
	{
		glm::vec3 lightPositionOnCamera = glm::vec3(viewMatrix * glm::vec4(light->transform.position, 1.0f));
		ShaderProgram::LoadUniform(_uniform_vs_fs_light_position, lightPositionOnCamera);
		ShaderProgram::LoadUniform(_uniform_vs_fs_light_diffuse, light->diffuseIntensity);
		ShaderProgram::LoadUniform(_uniform_vs_fs_light_specular, light->specularIntensity);
		ShaderProgram::LoadUniform(_uniform_vs_fs_light_ambient, light->ambientIntensity);
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_vs_fs_light_diffuse, glm::vec3(-1, -1, -1));
	}

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
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.diffuseMap.texture"), 0);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.specularMap.texture"), 1);
	ShaderProgram::LoadUniform(ShaderProgram::GetUniformLocation("fs_material.emissionMap.texture"), 2);
	Stop();

	return true;
}

void PhongShader::BindShaderAttributes()
{
	ShaderProgram::BindAttribute(Model::VBOlayout::POSITION, "attrib_position");
	ShaderProgram::BindAttribute(Model::VBOlayout::UVCOORD, "attrib_uvCoord");
	ShaderProgram::BindAttribute(Model::VBOlayout::NORMAL, "attrib_normal");
}
void PhongShader::GetShaderUniformLocations()
{
	_uniform_vs_ModelViewProjectionMatrix = ShaderProgram::GetUniformLocation("vs_ModelViewProjectionMatrix");
	_uniform_vs_modelViewMatrix = ShaderProgram::GetUniformLocation("vs_modelViewMatrix");
	_uniform_vs_normalMatrix = ShaderProgram::GetUniformLocation("vs_normalMatrix");

	_uniform_fs_material_diffuseMap_value = ShaderProgram::GetUniformLocation("fs_material.diffuseMap.value");
	_uniform_fs_material_specularMap_value = ShaderProgram::GetUniformLocation("fs_material.specularMap.value");
	_uniform_fs_material_emissionMap_value = ShaderProgram::GetUniformLocation("fs_material.emissionMap.value");

	_uniform_vs_fs_light_position = ShaderProgram::GetUniformLocation("vs_fs_light.position");
	_uniform_vs_fs_light_diffuse = ShaderProgram::GetUniformLocation("vs_fs_light.diffuse");
	_uniform_vs_fs_light_specular = ShaderProgram::GetUniformLocation("vs_fs_light.specular");
	_uniform_vs_fs_light_ambient = ShaderProgram::GetUniformLocation("vs_fs_light.ambient");
}

} } // namespace AX::Shader
