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
	GameObject() : mesh(nullptr), material(nullptr), _parent(nullptr) { }
	explicit GameObject(Asset::Mesh* mesh) : mesh(mesh), material(nullptr), _parent(nullptr) { }
	explicit GameObject(Asset::Mesh& mesh) : mesh(&mesh), material(nullptr), _parent(nullptr) { }
	GameObject(Asset::Mesh* mesh, Asset::Material* material) : mesh(mesh), material(material), _parent(nullptr) { }
	GameObject(Asset::Mesh& mesh, Asset::Material& material) : mesh(&mesh), material(&material), _parent(nullptr) { }
	
	void Render() const;
	void RenderOutline(glm::vec4 outlineColor, Tool::F32 outlineScale);

	void AddChild(GameObject& child) { _childList.push_back(&child); child._parent=this; }
	void AddChild(GameObject* child) { _childList.push_back(child); child->_parent=this; }
	GameObject* GetChild(Tool::SIZE index) const { return index<_childList.size() ? _childList.at(index) : nullptr; }
	Tool::SIZE GetChildCount() const { return _childList.size(); }
	void RemoveChildren() { _childList.clear(); }
	GameObject* GetParent() const { return _parent; }

	std::string name;
	Tool::U8 layer = 1;
	bool useTransparency = false;

	Transform transform;

	Asset::Mesh* mesh;
	Asset::Material* material;

private:
	std::vector<GameObject*> _childList;

	GameObject* _parent;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__GAME_OBJECT_H
