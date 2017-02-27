/**
 *	File: AXengine/Entity/DirectionalLight.h
 *	Purpose: Sun-like light
 */

#ifndef __AX__ENTITY__DIRECTIONAL_LIGHT_H
#define __AX__ENTITY__DIRECTIONAL_LIGHT_H

#include "AXengine/Entity/Light.h"

namespace AX { namespace Entity {

class DirectionalLight : public Light {
public:
	DirectionalLight() : Light(Type::DIRECTIONAL) { }

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__DIRECTIONAL_LIGHT_H
