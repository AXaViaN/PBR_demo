#include "AXengine/Gfx/Renderer.h"

#include "AXengine/Core/Window.h"
#include "AXengine/Asset/Material.h"
#include "AXengine/Entity/GameObject.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace AX { namespace Gfx {

const Tool::F32 Renderer::FOV = 66.6;
const Tool::F32 Renderer::NEAR_PLANE = 0.1f;
const Tool::F32 Renderer::FAR_PLANE = 1000.0f;

void Renderer::Clear(Tool::F32 red, Tool::F32 green, Tool::F32 blue)
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::PrepareScene()
{
	instance->_scene.ResetScene();

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Camera& camera)
{
	instance->_scene.ResetScene();
	instance->_scene.camera = &camera;

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Camera* camera)
{
	instance->_scene.ResetScene();
	instance->_scene.camera = camera;

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Light& light)
{
	instance->_scene.ResetScene();
	instance->_scene.AddLight(&light);

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Light* light)
{
	instance->_scene.ResetScene();
	instance->_scene.AddLight(light);

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Light** lightList, Tool::SIZE lightListSize)
{
	instance->_scene.ResetScene();
	instance->_scene.AddLight(lightList, lightListSize/sizeof(Entity::Light*));

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Camera& camera, const Entity::Light& light)
{
	instance->_scene.ResetScene();
	instance->_scene.camera = &camera;
	instance->_scene.AddLight(&light);

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Camera* camera, const Entity::Light* light)
{
	instance->_scene.ResetScene();
	instance->_scene.camera = camera;
	instance->_scene.AddLight(light);

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Camera& camera, const Entity::Light** lightList, Tool::SIZE lightListSize)
{
	instance->_scene.ResetScene();
	instance->_scene.camera = &camera;
	instance->_scene.AddLight(lightList, lightListSize/sizeof(Entity::Light*));

	prepareScene();
}
void Renderer::PrepareScene(const Entity::Camera* camera, const Entity::Light** lightList, Tool::SIZE lightListSize)
{
	instance->_scene.ResetScene();
	instance->_scene.camera = camera;
	instance->_scene.AddLight(lightList, lightListSize/sizeof(Entity::Light*));

	prepareScene();
}

void Renderer::PrepareShader(Shader::ShaderProgram* shaderProgram)
{
	if(instance->_shaderReadyMap.find(shaderProgram) == instance->_shaderReadyMap.end())
		instance->_shaderReadyMap.insert(std::pair<const Shader::ShaderProgram*, bool>(shaderProgram, false));

	if(instance->_shaderReadyMap[shaderProgram] == false)
	{
		shaderProgram->ProcessScene(instance->_scene);

		instance->_shaderReadyMap[shaderProgram] = true;
	}
}

void Renderer::Render(const Asset::Mesh& mesh)
{
	glBindVertexArray(mesh.GetVaoID());

	// Enable all the VBOs before rendering (-1 for index buffer)
	for( Tool::U32 vbo=0 ; vbo<mesh.GetVboCount()-1 ; vbo++ )
		glEnableVertexAttribArray(vbo);

	glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, nullptr);

	// Disable all the VBOs after rendering
	for( Tool::U32 vbo=0 ; vbo<mesh.GetVboCount()-1 ; vbo++ )
		glDisableVertexAttribArray(vbo);
}
void Renderer::Render(const Asset::Mesh& mesh, const Asset::Texture& texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());

	Render(mesh);
}

void Renderer::RegisterToBatch(const Entity::GameObject* gameObject)
{
	if(gameObject == nullptr || gameObject->mesh == nullptr)
		return;

	Shader::ShaderProgram* shader = nullptr;
	if(gameObject->material)
		shader = gameObject->material->shader;

	if(gameObject->useTransparency)
		instance->_transparentRenderBatch.push_back(const_cast<Entity::GameObject*>(gameObject));
	else
		instance->_objectRenderBatch[shader][gameObject->material].push_back(const_cast<Entity::GameObject*>(gameObject));
}
void Renderer::RegisterToBatch(const Entity::GameObject* gameObject, glm::vec4 outlineColor, Tool::F32 outlineScale)
{
	if(gameObject->mesh == nullptr)
		return;

	instance->_outlineRenderBatch.push_back({const_cast<Entity::GameObject*>(gameObject), outlineColor, outlineScale});
}
void Renderer::RenderBatch()
{
	// Render opaque objects first
	renderObjectBatch();

	// Render transparent objects
	renderTransparentBatch();

	// Render outlines
	renderOutlineBatch();
}

void Renderer::SetDebugMode(bool isDebugMode)
{
	if(isDebugMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	instance->_isDebugMode = isDebugMode;
}
void Renderer::SetFullScreen(bool isFullScreen)
{
	Core::Window::Instance().SetFullScreen(isFullScreen);
}
void Renderer::SetWindowTitle(const Tool::CHR* title)
{
	Core::Window::Instance().SetTitle(title);
}
void Renderer::SetWindowSize(glm::ivec2 size)
{
	Core::Window::Instance().SetWindowSize(size);
}

glm::mat4 Renderer::CreateProjectionMatrix(Tool::F32 fov, Tool::F32 nearPlane, Tool::F32 farPlane)
{
	glm::ivec2 windowSize = Core::Window::GetWindowSize();
	Tool::F32 aspectRatio = ((Tool::F32)windowSize.x)/((Tool::F32)windowSize.y);

	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

/***** PROTECTED *****/

void Renderer::Init()
{
	_isDebugMode = false;
	_projectionMatrix = CreateProjectionMatrix(FOV, NEAR_PLANE, FAR_PLANE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/***** PRIVATE *****/

void Renderer::renderObjectBatch()
{
	for( auto& shader_material : instance->_objectRenderBatch )
	{
		// Run & prepare shader
		if(shader_material.first)
			shader_material.first->Start();
		PrepareShader(shader_material.first);

		for( auto& material_object : shader_material.second )
		{
			// Process material if a shader is present
			if(shader_material.first)
				shader_material.first->ProcessMaterial(*material_object.first);

			// Process gameobject
			for( auto& object : material_object.second )
			{
				glStencilFunc(GL_ALWAYS, object->layer, 0xFF);

				// Apply parental transform
				Entity::Transform transform_ = object->transform;
				Entity::GameObject* parent = object->GetParent();
				while(parent)
				{
					object->transform += parent->transform;

					parent = parent->GetParent();
				}

				// Render
				renderObject(object);

				// Restore local transform
				object->transform = transform_;
			}
		}

		// Stop shader
		if(shader_material.first)
			shader_material.first->Stop();
	}

	instance->_objectRenderBatch.clear();
}
void Renderer::renderTransparentBatch()
{
	if(instance->_transparentRenderBatch.size() == 0)
		return;

	glm::vec3 cameraPosition;
	if(instance->_scene.camera)
		cameraPosition = instance->_scene.camera->transform.position;

	// Apply parental transform to transparent objects before sorting
	std::vector<Entity::Transform> transparentOriginalTransformList;
	for( auto& object : instance->_transparentRenderBatch )
	{
		transparentOriginalTransformList.push_back(object->transform);
		Entity::GameObject* parent = object->GetParent();
		while(parent)
		{
			object->transform += parent->transform;

			parent = parent->GetParent();
		}
	}

	// Sort objects from farther to closer
	std::sort(instance->_transparentRenderBatch.begin(), instance->_transparentRenderBatch.end(), 
			  [&cameraPosition](Entity::GameObject*& obj1, Entity::GameObject*& obj2){
		return glm::length(cameraPosition - obj1->transform.position) > glm::length(cameraPosition - obj2->transform.position);
	});

	// Render objects
	glDisable(GL_CULL_FACE);
	Shader::ShaderProgram* shader = nullptr;
	for( Tool::SIZE i=0 ; i<instance->_transparentRenderBatch.size() ; i++ )
	{
		Entity::GameObject* object = instance->_transparentRenderBatch[i];

		// If there is no shader, there can't be any transparency
		if(object->material == nullptr || object->material->shader == nullptr)
			continue;

		if(shader != object->material->shader)
		{
			if(shader)
				shader->Stop();

			shader = object->material->shader;
			shader->Start();
			PrepareShader(shader);
		}

		glStencilFunc(GL_ALWAYS, instance->_transparentRenderBatch[i]->layer, 0xFF);

		shader->ProcessMaterial(*object->material);
		renderObject(object);

		instance->_transparentRenderBatch[i]->transform = transparentOriginalTransformList[i];
	}
	if(shader)
		shader->Stop();
	glEnable(GL_CULL_FACE);

	instance->_transparentRenderBatch.clear();
}
void Renderer::renderOutlineBatch()
{
	if(instance->_outlineRenderBatch.size() == 0)
		return;

	glDisable(GL_DEPTH_TEST);
	// Stencil writing is not disabled. Outlines will fill the buffer with
	// layer info. This is done to adjust multiple outlined objects with
	// different layers.

	Asset::Material material;
	material.shader->Start();
	PrepareShader(material.shader);
	for( auto& outlineObject : instance->_outlineRenderBatch )
	{
		glStencilFunc(GL_GREATER, outlineObject.object->layer, 0xFF);

		Asset::Material* material_ = outlineObject.object->material;
		Entity::Transform transform_ = outlineObject.object->transform;
		Entity::GameObject* parent = outlineObject.object->GetParent();
		while(parent)
		{
			outlineObject.object->transform += parent->transform;

			parent = parent->GetParent();
		}

		outlineObject.object->transform.Scale(1.0 + outlineObject.scale);
		outlineObject.object->material = &material;
		material.diffuseMap.value = outlineObject.color;

		material.shader->ProcessMaterial(material);
		renderObject(outlineObject.object);

		outlineObject.object->transform = transform_;
		outlineObject.object->material = material_;
	}
	material.shader->Stop();

	glEnable(GL_DEPTH_TEST);

	instance->_outlineRenderBatch.clear();
}
void Renderer::renderObject(const Entity::GameObject* object)
{
	if(object->material && object->material->shader)
	{
		object->material->shader->ProcessTransform(object->transform);
		Render(*object->mesh);
	}
	else
	{
		Render(*object->mesh);
	}
}

void Renderer::prepareScene()
{
	for( auto& readyMap : instance->_shaderReadyMap )
		readyMap.second = false;
}

} } // namespace AX::Render
