/**
 *	File: AXengine/Entity/Light.h
 *	Purpose: Simulate virtual light
 */

#ifndef __AX__ENTITY__LIGHT_H
#define __AX__ENTITY__LIGHT_H

#include "AXengine/Entity/Transform.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX { namespace Entity {

class Light {
public:
	void SetColor(Tool::F32 r, Tool::F32 g, Tool::F32 b) { color.r = r; color.g = g; color.b = b; }

	Transform transform;
	glm::vec3 color;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__LIGHT_H
