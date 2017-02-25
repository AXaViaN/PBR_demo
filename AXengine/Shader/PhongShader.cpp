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
		ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec3(0, 1, 0));
	}
	else
	{
		// Process material
		if(gameObject.material->diffuseMap.texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gameObject.material->diffuseMap.texture->GetTextureID());
			ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, glm::vec3(-1, -1, -1));
		}
		else
		{
			ShaderProgram::LoadUniform(_uniform_fs_diffuseValue, gameObject.material->diffuseMap.value);
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
		ShaderProgram::LoadUniform(_uniform_vs_lightPositionOnCamera, lightPositionOnCamera);
		ShaderProgram::LoadUniform(_uniform_fs_lightColor, light->color);
	}
	else
	{
		ShaderProgram::LoadUniform(_uniform_fs_lightColor, glm::vec3(-1, -1, -1));
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
	_uniform_vs_lightPositionOnCamera = ShaderProgram::GetUniformLocation("vs_lightPositionOnCamera");

	_uniform_fs_diffuseValue = ShaderProgram::GetUniformLocation("fs_diffuseValue");
	_uniform_fs_lightColor = ShaderProgram::GetUniformLocation("fs_lightColor");
}

} } // namespace AX::Shader
