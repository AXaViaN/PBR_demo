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
	Transform transform;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__CAMERA_H
