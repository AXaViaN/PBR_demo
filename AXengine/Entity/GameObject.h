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

	Transform transform;

	Asset::Mesh* mesh;
	Asset::Material* material;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__GAME_OBJECT_H
