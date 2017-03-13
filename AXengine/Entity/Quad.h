/**
 *	File: AXengine/Entity/Quad.h
 *	Purpose: Represent a 2D quad
 */

#ifndef __AX__ENTITY__QUAD_H
#define __AX__ENTITY__QUAD_H

#include "AXengine/Asset/Material.h"
#include "AXengine/Asset/Mesh.h"
#include "AXengine/Entity/Transform.h"
#include "AXengine/Gfx/Renderer2D.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Entity {

class Quad {
public:
	Quad();

	void Render()
	{
		Gfx::Renderer2D::RegisterToBatch(this);
	}
	void RenderImmediate()
	{
		Gfx::Renderer2D::Activate2DMode();
		Gfx::Renderer2D::Render(this);
		Gfx::Renderer2D::Deactivate2DMode();
	}

	Transform transform;

	Asset::Material material;
	static Asset::Mesh mesh;

protected:
	friend class Core::Engine;
	static void InitMesh();
	static void DisposeMesh()
	{
		mesh.Dispose();
	}

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__QUAD_H
