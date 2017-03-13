#include "AXengine/Gfx/Renderer2D.h"

#include "AXengine/Entity/Quad.h"
#include <GL/glew.h>

namespace AX { namespace Gfx {

void Renderer2D::Activate2DMode()
{
	glDisable(GL_DEPTH_TEST);
	instance->_isRenderer3DdebugMode = Renderer::GetDebugMode();
	Renderer::SetDebugMode(false);
}
void Renderer2D::Deactivate2DMode()
{
	Renderer::SetDebugMode(instance->_isRenderer3DdebugMode );
	glEnable(GL_DEPTH_TEST);
}

void Renderer2D::Render(const Entity::Quad& quad)
{
	Render(&quad);
}
void Renderer2D::Render(const Entity::Quad* quad)
{
	if(quad == nullptr)
		return;

	quad->material.shader->Start();

	quad->material.shader->ProcessTransform(quad->transform);
	quad->material.shader->ProcessMaterial(quad->material);
	Renderer::Render(quad->mesh);

	quad->material.shader->Stop();
}

void Renderer2D::RegisterToBatch(const Entity::Quad* quad)
{
	if(quad)
		instance->_quadRenderBatch.push_back(quad);
}
void Renderer2D::RenderBatch()
{
	if(instance==nullptr)
		return;

	Activate2DMode();

	for( auto& quad : instance->_quadRenderBatch )
		Render(quad);

	Deactivate2DMode();

	instance->_quadRenderBatch.clear();
}

} } // namespace AX::Gfx
