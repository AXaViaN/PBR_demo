#include "AXengine/Entity/GameObject.h"

#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Model/Material.h"
#include "AXengine/Model/Mesh.h"
#include "AXengine/Shader/ShaderProgram.h"

namespace AX { namespace Entity {

void GameObject::Render() const
{
	Render(nullptr, nullptr, 0);
}
void GameObject::Render(const Camera& camera) const
{
	Render(&camera, nullptr, 0);
}
void GameObject::Render(const Camera* camera) const
{
	Render(camera, nullptr, 0);
}
void GameObject::Render(const Light& light) const
{
	const Light* lightList[] = {&light};
	Render(nullptr, lightList, sizeof(lightList));
}
void GameObject::Render(const Light* light) const
{
	const Light* lightList[] = {light};
	Render(nullptr, lightList, sizeof(lightList));
}
void GameObject::Render(const Light** lightList, Tool::SIZE lightListSize) const
{
	Render(nullptr, lightList, lightListSize);
}
void GameObject::Render(const Camera& camera, const Light& light) const
{
	const Light* lightList[] = {&light};
	Render(&camera, lightList, sizeof(lightList));
}
void GameObject::Render(const Camera* camera, const Light* light) const
{
	const Light* lightList[] = {light};
	Render(camera, lightList, sizeof(lightList));
}
void GameObject::Render(const Camera& camera, const Light** lightList, Tool::SIZE lightListSize) const
{
	Render(&camera, lightList, lightListSize);
}
void GameObject::Render(const Camera* camera, const Light** lightList, Tool::SIZE lightListSize) const
{
	if(mesh)
	{
		if(material && material->shader)
		{
			material->shader->Start();
			material->shader->ProcessGameObject(*this, camera, lightList, lightListSize/sizeof(Light*));
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
