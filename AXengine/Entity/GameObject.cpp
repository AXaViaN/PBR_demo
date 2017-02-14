#include "AXengine/Entity/GameObject.h"

#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Model/Material.h"
#include "AXengine/Model/Mesh.h"
#include "AXengine/Shader/ShaderProgram.h"

namespace AX { namespace Entity {

void GameObject::Render()
{
	if(mesh)
	{
		if(material && material->shader)
		{
			material->shader->Start();
				material->shader->ProcessMaterial(*material);
				Gfx::Renderer::Render(*mesh);
			material->shader->Stop();
		}
		else
		{
			Gfx::Renderer::Render(*mesh);
		}
	}
}

} } // namespace AX::Entity
