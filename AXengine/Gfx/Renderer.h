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

	static void RegisterToBatch(const Entity::GameObject* gameObject);
	static void RegisterToBatch(const Entity::GameObject* gameObject, glm::vec4 outlineColor, Tool::F32 outlineScale);
	static void RenderBatch();

	/**
	 * In debug mode objects are rendered as wireframes
	 */
	static void SetDebugMode(bool isDebugMode);
	static void SetFullScreen(bool isFullScreen);
	static void SetWindowTitle(const Tool::CHR* title);
	static void SetWindowSize(glm::ivec2 size);

	static glm::mat4 CreateProjectionMatrix(Tool::F32 fov, Tool::F32 nearPlane, Tool::F32 farPlane);
	static const glm::mat4& GetDefaultProjectionMatrix() { return instance->_projectionMatrix; }

	static const Tool::F32 FOV;
	static const Tool::F32 NEAR_PLANE;
	static const Tool::F32 FAR_PLANE;

protected:
	/**
	 * Init is only visible for Engine
	 * 
	 * This is done to prevent API users to call this method.
	 */
	friend class Core::Engine;
	void Init();

private:
	struct BatchOutlineObject {
		Entity::GameObject* object;
		glm::vec4 color;
		Tool::F32 scale;
	};

	static void renderObject(const Entity::GameObject* object);

private:
	static void prepareScene();

	std::map<const Shader::ShaderProgram*, bool> _shaderReadyMap;
	std::vector<Entity::GameObject*> _objectRenderBatch;
	std::vector<Entity::GameObject*> _transparentRenderBatch;
	std::vector<BatchOutlineObject> _outlineRenderBatch;

	Entity::Scene _scene;
	glm::mat4 _projectionMatrix;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__RENDERER_H
