/**
 *	File: AXengine/Entity/GameObject.h
 *	Purpose: Instance of a model
 */

#ifndef __AX__ENTITY__GAME_OBJECT_H
#define __AX__ENTITY__GAME_OBJECT_H

#include "AXengine/Entity/Camera.h"
#include "AXengine/Entity/Light.h"
#include "AXengine/Entity/Transform.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

namespace AX { namespace Asset {
class Material;
class Mesh;
} }

namespace AX { namespace Entity {

class GameObject {
public:
	GameObject() : mesh(nullptr), material(nullptr) { }
	explicit GameObject(Asset::Mesh* mesh) : mesh(mesh), material(nullptr) { }
	explicit GameObject(Asset::Mesh& mesh) : mesh(&mesh), material(nullptr) { }
	GameObject(Asset::Mesh* mesh, Asset::Material* material) : mesh(mesh), material(material) { }
	GameObject(Asset::Mesh& mesh, Asset::Material& material) : mesh(&mesh), material(&material) { }
	
	void Render() const;
	void RenderOutline(glm::vec3 outlineColor, Tool::F32 outlineScale);

	void AddChild(GameObject& child) { _childList.push_back(&child); }
	void AddChild(GameObject* child) { _childList.push_back(child); }
	GameObject* GetChild(Tool::SIZE index) const { return index<_childList.size() ? _childList.at(index) : nullptr; }
	Tool::SIZE GetChildCount() const { return _childList.size(); }

	std::string name;
	Tool::U8 layer = 1;

	Transform transform;

	Asset::Mesh* mesh;
	Asset::Material* material;

private:
	void renderObject() const;

	void setOutlinedMaterial(GameObject* object, Asset::Material& material);
	void restoreMaterial(GameObject* object);

	std::vector<GameObject*> _childList;
	std::map<GameObject*, Asset::Material*> _nonOutlinedMaterialMap;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__GAME_OBJECT_H
