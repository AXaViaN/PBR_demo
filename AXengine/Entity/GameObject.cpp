#include "AXengine/Entity/GameObject.h"

#include "AXengine/Asset/Material.h"
#include "AXengine/Asset/Mesh.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Shader/ShaderProgram.h"
#include <GL/glew.h>

namespace AX { namespace Entity {

void GameObject::Render() const
{
	Gfx::Renderer::RegisterToBatch(this);

	for( auto& childObject : _childList )
	{
		childObject->layer = layer;

		childObject->Render();
	}
}
void GameObject::RenderOutline(glm::vec4 outlineColor, Tool::F32 outlineScale)
{
	Gfx::Renderer::RegisterToBatch(this, outlineColor, outlineScale);

	for( auto& childObject : _childList )
	{
		childObject->_parent = const_cast<GameObject*>(this);
		childObject->layer = layer;

		childObject->RenderOutline(outlineColor, outlineScale);
	}
}

} } // namespace AX::Entity
