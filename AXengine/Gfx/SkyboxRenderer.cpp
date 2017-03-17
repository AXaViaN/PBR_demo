#include "AXengine/Gfx/SkyboxRenderer.h"

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Gfx/Renderer.h"
#include <GL/glew.h>

namespace AX { namespace Gfx {

Entity::Cubemap* SkyboxRenderer::_skybox = nullptr;

void SkyboxRenderer::RenderSkybox()
{
	if(_skybox == nullptr)
		return;

	glDisable(GL_DEPTH_TEST);
	_skybox->material.shader->Start();

	Renderer::PrepareShader(_skybox->material.shader);
	_skybox->material.shader->ProcessMaterial(_skybox->material);
	
	glBindVertexArray(_skybox->mesh.GetVaoID());
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);

	_skybox->material.shader->Stop();
	glEnable(GL_DEPTH_TEST);

	_skybox = nullptr;
}

} } // namespace AX::Gfx
