/**
 *	File: AXengine/Entity/PointLight.h
 *	Purpose: Light from a point (light bulb etc.)
 */

#ifndef __AX__ENTITY__POINT_LIGHT_H
#define __AX__ENTITY__POINT_LIGHT_H

#include "AXengine/Entity/Light.h"

namespace AX { namespace Entity {

class PointLight : public Light {
public:
	PointLight() : Light(Type::POINT) { }

	void SetPosition(Tool::F32 x, Tool::F32 y, Tool::F32 z) { position.x = x; position.y = y; position.z = z; }

	glm::vec3 position;

	// Attenuation attributes
	Tool::F32 constant = 1.0f;
	Tool::F32 linear = 0.1f;
	Tool::F32 quadric = 0.05f;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__POINT_LIGHT_H
