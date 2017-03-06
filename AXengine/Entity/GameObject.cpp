#include "AXengine/Entity/GameObject.h"

#include "AXengine/Asset/Material.h"
#include "AXengine/Asset/Mesh.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Shader/ShaderProgram.h"
#include <GL/glew.h>

namespace AX { namespace Entity {

void GameObject::Render() const
{
	glStencilFunc(GL_ALWAYS, layer, 0xFF);

	renderObject();
}
void GameObject::RenderOutline(glm::vec3 outlineColor, Tool::F32 outlineScale)
{
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_GREATER, layer, 0xFF);
	// Stencil writing is not disabled. Outlines will fill the buffer with
	// layer info. This is done to adjust multiple outlined object with
	// different layers.
	
	Transform transform_ = transform;
	transform.Scale(1.0 + outlineScale);
	Asset::Material material;
	material.diffuseMap.value = outlineColor;
	setOutlinedMaterial(this, material);

	renderObject();

	restoreMaterial(this);
	transform = transform_;

	glEnable(GL_DEPTH_TEST);
}

/***** PRIVATE *****/

void GameObject::renderObject() const
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
		childObject->layer = layer;
		Transform childTransform = childObject->transform;

		childObject->transform.position += transform.position;
		childObject->transform.rotation += transform.rotation;
		childObject->transform.scale *= transform.scale;
		childObject->renderObject();

		childObject->transform = childTransform;
	}
}

void GameObject::setOutlinedMaterial(GameObject* object, Asset::Material& material)
{
	_nonOutlinedMaterialMap[object] = object->material;
	object->material = &material;
	
	for( auto& childObject : object->_childList )
		setOutlinedMaterial(childObject, material);
}
void GameObject::restoreMaterial(GameObject* object)
{
	object->material = _nonOutlinedMaterialMap[object];

	for( auto& childObject : object->_childList )
		restoreMaterial(childObject);
}

} } // namespace AX::Entity
