/**
 *	File: AXengine/Model/PhongMaterial.h
 *	Purpose: Basic lighting information for PhongShader
 */

#ifndef __AX__MODEL__PHONG_MATERIAL_H
#define __AX__MODEL__PHONG_MATERIAL_H

#include "AXengine/Model/Material.h"

namespace AX { namespace Model {

class PhongMaterial : public Material {
public:
	PhongMaterial();

	TextureMap<glm::vec3> specularMap;
	TextureMap<glm::vec3> emissionMap;

};

} } // namespace AX::Model

#endif // __AX__MODEL__PHONG_MATERIAL_H
