/**
 *	File: AXengine/Gfx/Renderer.h
 *	Purpose: Handle rendering to window
 */

#ifndef __AX__GFX__RENDERER_H
#define __AX__GFX__RENDERER_H

#include "AXengine/Model/Mesh.h"
#include "AXengine/Model/Texture.h"
#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Gfx {

class Renderer : public Tool::Singleton<Renderer> {
public:
	static void Clear(Tool::F32 red, Tool::F32 green, Tool::F32 blue);

	static void Render(const Model::Mesh& mesh);
	static void Render(const Model::Mesh& mesh, const Model::Texture& texture);

	/**
	 * In debug mode objects are rendered as wireframes
	 */
	static void SetDebugMode(bool isDebugMode);

	static glm::mat4 CreateProjectionMatrix(Tool::F32 fov, Tool::F32 nearPlane, Tool::F32 farPlane);
	static const glm::mat4& GetDefaultProjectionMatrix() { return instance->_projectionMatrix; }

	const Tool::F32 FOV = 66.6;
	const Tool::F32 NEAR_PLANE = 0.1f;
	const Tool::F32 FAR_PLANE = 1000.0f;

protected:
	/**
	 * Init is only visible for Engine
	 * 
	 * This is done to prevent API users to call this method.
	 */
	friend class Core::Engine;
	void Init();

private:
	glm::mat4 _projectionMatrix;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__RENDERER_H
