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
	void SetDiffuseIntensity(Tool::F32 r, Tool::F32 g, Tool::F32 b) { diffuseIntensity.r = r; diffuseIntensity.g = g; diffuseIntensity.b = b; }
	void SetSpecularIntensity(Tool::F32 r, Tool::F32 g, Tool::F32 b) { specularIntensity.r = r; specularIntensity.g = g; specularIntensity.b = b; }
	void SetAmbientIntensity(Tool::F32 r, Tool::F32 g, Tool::F32 b) { ambientIntensity.r = r; ambientIntensity.g = g; ambientIntensity.b = b; }

	Transform transform;
	glm::vec3 diffuseIntensity = glm::vec3(0.9, 0.9, 0.9);
	glm::vec3 specularIntensity = glm::vec3(1, 1, 1);
	glm::vec3 ambientIntensity = glm::vec3(0.1, 0.1, 0.1);

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__LIGHT_H
