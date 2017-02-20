/**
 *	File: AXengine/Entity/FreeCamera.h
 *	Purpose: Simulate virtual camera in 3D space
 *	
 *	Controls are static for now. WASD+LSHIFT+LCTRL + Mouse movement
 */

#ifndef __AX__ENTITY_FREE_CAMERA_H
#define __AX__ENTITY_FREE_CAMERA_H

#include "AXengine/Entity/Camera.h"

namespace AX { namespace Entity {

class FreeCamera : public Camera {
public:
	FreeCamera() : _movementSpeed(1), _rotationSpeed(1) { }

	void Update();

	void SetMovementSpeed(Tool::F32 movementSpeed) { _movementSpeed = movementSpeed; }
	void SetRotationSpeed(Tool::F32 rotationSpeed) { _rotationSpeed = rotationSpeed; }

private:
	Tool::F32 _movementSpeed;
	Tool::F32 _rotationSpeed;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY_FREE_CAMERA_H
