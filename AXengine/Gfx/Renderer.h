/**
 *	File: AXengine/Gfx/Renderer.h
 *	Purpose: Handle rendering to window
 */

#ifndef __AX__GFX__RENDERER_H
#define __AX__GFX__RENDERER_H

#include "AXengine/Asset/Mesh.h"
#include "AXengine/Asset/Texture.h"
#include "AXengine/Entity/Scene.h"
#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <map>

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Gfx {

class Renderer : public Tool::Singleton<Renderer> {
public:
	static void Clear(Tool::F32 red, Tool::F32 green, Tool::F32 blue);

	static void PrepareScene();
	static void PrepareScene(const Entity::Camera& camera);
	static void PrepareScene(const Entity::Camera* camera);
	static void PrepareScene(const Entity::Light& light);
	static void PrepareScene(const Entity::Light* light);
	static void PrepareScene(const Entity::Light** lightList, Tool::SIZE lightListSize);
	static void PrepareScene(const Entity::Camera& camera, const Entity::Light& light);
	static void PrepareScene(const Entity::Camera* camera, const Entity::Light* light);
	static void PrepareScene(const Entity::Camera& camera, const Entity::Light** lightList, Tool::SIZE lightListSize);
	static void PrepareScene(const Entity::Camera* camera, const Entity::Light** lightList, Tool::SIZE lightListSize);

	static void PrepareShader(Shader::ShaderProgram* shaderProgram);

	static void Render(const Asset::Mesh& mesh);
	static void Render(const Asset::Mesh& mesh, const Asset::Texture& texture);

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
	static void prepareScene();

	glm::mat4 _projectionMatrix;

	std::map<const Shader::ShaderProgram*, bool> _shaderReadyMap;
	Entity::Scene _scene;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__RENDERER_H
