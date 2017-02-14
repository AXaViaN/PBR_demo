/**
 *	File: AXengine/Entity/GameObject.h
 *	Purpose: Instance of a model
 */

#ifndef __AX__ENTITY__GAME_OBJECT_H
#define __AX__ENTITY__GAME_OBJECT_H

#include <glm/glm.hpp>

namespace AX { namespace Model {
class Material;
class Mesh;
} }

namespace AX { namespace Entity {

class GameObject {
public:
	struct Transform {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

public:
	GameObject() : mesh(nullptr), material(nullptr) { }
	explicit GameObject(Model::Mesh* mesh) : mesh(mesh), material(nullptr) { }
	explicit GameObject(Model::Mesh& mesh) : mesh(&mesh), material(nullptr) { }
	GameObject(Model::Mesh* mesh, Model::Material* material) : mesh(mesh), material(material) { }
	GameObject(Model::Mesh& mesh, Model::Material& material) : mesh(&mesh), material(&material) { }

	void Render() const;

	Transform transform;

	Model::Mesh* mesh;
	Model::Material* material;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__GAME_OBJECT_H
