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
#include <vector>

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

	void AddChild(GameObject& child) { _childList.push_back(&child); }
	void AddChild(GameObject* child) { _childList.push_back(child); }
	GameObject* GetChild(Tool::SIZE index) { return index<_childList.size() ? _childList.at(index) : nullptr; }

	Transform transform;

	Asset::Mesh* mesh;
	Asset::Material* material;

private:
	std::vector<GameObject*> _childList;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__GAME_OBJECT_H
