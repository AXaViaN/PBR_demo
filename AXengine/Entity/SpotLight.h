/**
 *	File: AXengine/Entity/SpotLight.h
 *	Purpose: Directional point light (flashlight etc.)
 */

#ifndef __AX__ENTITY__SPOT_LIGHT_H
#define __AX__ENTITY__SPOT_LIGHT_H

#include "AXengine/Entity/Light.h"

namespace AX { namespace Entity {

class SpotLight : public Light {
public:
	SpotLight() : Light(Type::SPOT) { }

	void SetPosition(Tool::F32 x, Tool::F32 y, Tool::F32 z) { position.x = x; position.y = y; position.z = z; }
	void SetDirection(Tool::F32 x, Tool::F32 y, Tool::F32 z) { direction.x = x; direction.y = y; direction.z = z; }

	glm::vec3 position;
	glm::vec3 direction;

	// Attenuation attributes
	Tool::F32 cutoff = 15.0f;
	Tool::F32 outerCutoffFactor = 1.25f;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__SPOT_LIGHT_H
