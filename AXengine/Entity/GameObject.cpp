#include "AXengine/Entity/GameObject.h"

#include "AXengine/Asset/Material.h"
#include "AXengine/Asset/Mesh.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Shader/ShaderProgram.h"

namespace AX { namespace Entity {

void GameObject::Render() const
{
	if(mesh)
	{
		if(material && material->shader)
		{
			material->shader->Start();

			Gfx::Renderer::PrepareShader(material->shader);
			material->shader->ProcessGameObject(*this);
			Gfx::Renderer::Render(*mesh);

			material->shader->Stop();
		}
		else
		{
			Gfx::Renderer::Render(*mesh);
		}
	}

	// Render child objects with relative transforms
	for( auto& childObject : _childList )
	{
		Transform childTransform = childObject->transform;

		childObject->transform.position += transform.position;
		childObject->transform.rotation += transform.rotation;
		childObject->transform.scale *= transform.scale;
		childObject->Render();

		childObject->transform = childTransform;
	}
}

} } // namespace AX::Entity
