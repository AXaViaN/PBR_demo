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

	// Attenuation attributes
	Tool::F32 cutoff = 15.0f;
	Tool::F32 outerCutoffFactor = 1.25f;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__SPOT_LIGHT_H
