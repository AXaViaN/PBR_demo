/**
 *	File: AXengine/Entity/Camera.h
 *	Purpose: Represent a virtual camera
 */

#ifndef __AX__ENTITY__CAMERA_H
#define __AX__ENTITY__CAMERA_H

#include "AXengine/Entity/Transform.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX { namespace Entity {

class Camera {
public:
	const glm::vec3& GetForwardDirection() const { return forward; }
	const glm::vec3& GetUpDirection() const { return up; }

	Transform transform;

protected:
	Camera() { }

	glm::vec3 forward = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__CAMERA_H
