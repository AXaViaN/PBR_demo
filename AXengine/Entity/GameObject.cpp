#include "AXengine/Entity/GameObject.h"

#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Model/Material.h"
#include "AXengine/Model/Mesh.h"
#include "AXengine/Shader/ShaderProgram.h"

namespace AX { namespace Entity {

void GameObject::Render() const
{
	Render(nullptr, nullptr);
}
void GameObject::Render(const Camera& camera) const
{
	Render(&camera, nullptr);
}
void GameObject::Render(const Camera* camera) const
{
	Render(camera, nullptr);
}
void GameObject::Render(const Light& light) const
{
	Render(nullptr, &light);
}
void GameObject::Render(const Light* light) const
{
	Render(nullptr, light);
}
void GameObject::Render(const Camera& camera, const Light& light) const
{
	Render(&camera, &light);
}
void GameObject::Render(const Camera* camera, const Light* light) const
{
	if(mesh)
	{
		if(material && material->shader)
		{
			material->shader->Start();
			material->shader->ProcessGameObject(*this, camera, light);
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
