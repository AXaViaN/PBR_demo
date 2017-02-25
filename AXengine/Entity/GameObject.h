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

namespace AX { namespace Model {
class Material;
class Mesh;
} }

namespace AX { namespace Entity {

class GameObject {
public:
	GameObject() : mesh(nullptr), material(nullptr) { }
	explicit GameObject(Model::Mesh* mesh) : mesh(mesh), material(nullptr) { }
	explicit GameObject(Model::Mesh& mesh) : mesh(&mesh), material(nullptr) { }
	GameObject(Model::Mesh* mesh, Model::Material* material) : mesh(mesh), material(material) { }
	GameObject(Model::Mesh& mesh, Model::Material& material) : mesh(&mesh), material(&material) { }

	void Render() const;
	void Render(const Camera& camera) const;
	void Render(const Camera* camera) const;
	void Render(const Light& light) const;
	void Render(const Light* light) const;
	void Render(const Camera& camera, const Light& light) const;
	void Render(const Camera* camera, const Light* light) const;

	Transform transform;

	Model::Mesh* mesh;
	Model::Material* material;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__GAME_OBJECT_H
