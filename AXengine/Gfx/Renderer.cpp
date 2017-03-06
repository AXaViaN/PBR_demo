#include "AXengine/Gfx/Renderer.h"

#include "AXengine/Core/Window.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

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

void Renderer::SetDebugMode(bool isDebugMode)
{
	if(isDebugMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	_projectionMatrix = CreateProjectionMatrix(FOV, NEAR_PLANE, FAR_PLANE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
}

/***** PRIVATE *****/

void Renderer::prepareScene()
{
	for( auto& readyMap : instance->_shaderReadyMap )
		readyMap.second = false;
}

} } // namespace AX::Render
