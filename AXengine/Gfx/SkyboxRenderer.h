/**
 *	File: AXengine/Gfx/SkyboxRenderer.h
 *	Purpose: Handle rendering cubemap skybox
 */

#ifndef __AX__GFX__SKYBOX_RENDERER_H
#define __AX__GFX__SKYBOX_RENDERER_H

#include "AXengine/Entity/Cubemap.h"

namespace AX { namespace Gfx {

class SkyboxRenderer {
public:
	static void Render(Entity::Cubemap& skybox) { _skybox = &skybox; }

	static void RenderSkybox();

private:
	static Entity::Cubemap* _skybox;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__SKYBOX_RENDERER_H
