/**
 *	File: AXengine/Entity/Transform.h
 *	Purpose: Represent a 3D transformation information
 */

#ifndef __AX__ENTITY__TRANSFORM_H
#define __AX__ENTITY__TRANSFORM_H

#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX { namespace Entity {

class Transform {
public:
	void SetPosition(Tool::F32 x, Tool::F32 y, Tool::F32 z) { position.x = x; position.y = y; position.z = z; }
	void SetRotation(Tool::F32 x, Tool::F32 y, Tool::F32 z) { rotation.x = x; rotation.y = y; rotation.z = z; }
	void SetScale(Tool::F32 scale) { this->scale.x = scale; this->scale.y = scale; this->scale.z = scale; }
	void SetScale(Tool::F32 x, Tool::F32 y, Tool::F32 z) { scale.x = x; scale.y = y; scale.z = z; }

	void Translate(Tool::F32 x, Tool::F32 y, Tool::F32 z) { position.x += x; position.y += y; position.z += z; }
	void Translate(glm::vec3 translation) { position += translation; }

	void Rotate(Tool::F32 x, Tool::F32 y, Tool::F32 z) { rotation.x += x; rotation.y += y; rotation.z += z; }
	void Rotate(glm::vec3 axis, Tool::F32 angle) { rotation += axis*angle; }
	void Rotate(glm::vec3 rotation) { this->rotation += rotation; }

	void Scale(Tool::F32 scale) { this->scale *= scale; }
	void Scale(glm::vec3 scale) { this->scale *= scale; }

	Transform operator+(const Transform& other)
	{
		Transform transform;
		transform.position = this->position + other.position;
		transform.rotation = this->rotation + other.rotation;
		transform.scale = this->scale * other.scale;
		return transform;
	}
	Transform& operator+=(const Transform& other)
	{
		position += other.position;
		rotation += other.rotation;
		scale *= other.scale;
		return *this;
	}
	Transform operator-(const Transform& other)
	{
		Transform transform;
		transform.position = this->position - other.position;
		transform.rotation = this->rotation - other.rotation;
		if(other.scale.x!=0 && other.scale.y!=0 && other.scale.z!=0)
			transform.scale = this->scale / other.scale;
		return transform;
	}
	Transform& operator-=(const Transform& other)
	{
		position -= other.position;
		rotation -= other.rotation;
		if(other.scale.x==0 || other.scale.y==0 || other.scale.z==0)
			scale = glm::vec3();
		else
			scale /= other.scale;
		return *this;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1, 1, 1);

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__TRANSFORM_H
