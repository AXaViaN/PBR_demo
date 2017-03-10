/**
 *	File: AXengine/Gfx/Renderer2D.h
 *	Purpose: Handle 2D rendering to window
 */

#ifndef __AX__GFX__RENDERER2D_H
#define __AX__GFX__RENDERER2D_H

#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AX { namespace Gfx {

class Renderer2D {
public:

	static glm::mat4 CreateProjectionMatrix(Tool::F32 width, Tool::F32 height)
	{
		return glm::ortho(0.0f, width, 0.0f, height, Renderer::NEAR_PLANE, Renderer::FAR_PLANE);
	}

};

} } // namespace AX::Gfx

#endif // __AX__GFX__RENDERER2D_H
