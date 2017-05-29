/**
 *	File: AXengine/Asset/PhongMaterial.h
 *	Purpose: Basic lighting information for PhongShader
 */

#ifndef __AX__ASSET__PHONG_MATERIAL_H
#define __AX__ASSET__PHONG_MATERIAL_H

#include "AXengine/Asset/Material.h"

namespace AX { namespace Asset {

class PhongMaterial : public Material {
public:
	PhongMaterial();

	TextureMap<glm::vec3> specularMap;
	TextureMap<glm::vec3> normalMap;
	Tool::F32 shininess;

};

} } // namespace AX::Asset

#endif // __AX__ASSET__PHONG_MATERIAL_H
