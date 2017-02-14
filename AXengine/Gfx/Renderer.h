/**
 *	File: AXengine/Gfx/Renderer.h
 *	Purpose: Handle rendering to window
 */

#ifndef __AX__GFX__RENDERER_H
#define __AX__GFX__RENDERER_H

#include "AXengine/Model/Mesh.h"
#include "AXengine/Model/Texture.h"

namespace AX { namespace Gfx {

class Renderer {
public:
	static void Clear(Tool::F32 red, Tool::F32 green, Tool::F32 blue);

	static void Render(const Model::Mesh& mesh);
	static void Render(const Model::Mesh& mesh, const Model::Texture& texture);

};

} } // namespace AX::Gfx

#endif // __AX__GFX__RENDERER_H
