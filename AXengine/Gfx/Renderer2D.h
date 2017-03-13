/**
 *	File: AXengine/Gfx/Renderer2D.h
 *	Purpose: Handle 2D rendering to window
 */

#ifndef __AX__GFX__RENDERER2D_H
#define __AX__GFX__RENDERER2D_H

#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace AX {
namespace Core { class Engine; }
namespace Entity { class Quad; }
}

namespace AX { namespace Gfx {

class Renderer2D : public Tool::Singleton<Renderer2D> {
public:
	static void Activate2DMode();
	static void Deactivate2DMode();

	static void Render(const Entity::Quad& quad);
	static void Render(const Entity::Quad* quad);

	static void RegisterToBatch(const Entity::Quad* quad);
	static void RenderBatch();

	static glm::mat4 CreateProjectionMatrix(Tool::F32 width, Tool::F32 height)
	{
		return glm::ortho(0.0f, width, 0.0f, height, Renderer::NEAR_PLANE, Renderer::FAR_PLANE);
	}

protected:
	/**
	 * Init is only visible for Engine
	 * 
	 * This is done to prevent API users to call this method.
	 */
	friend class Core::Engine;
	void Init() { }

private:
	std::vector<const Entity::Quad*> _quadRenderBatch;

	bool _isRenderer3DdebugMode;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__RENDERER2D_H
