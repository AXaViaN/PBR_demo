/**
 *	File: AXengine/Asset/PBRMaterial.h
 *	Purpose: TextureMap information for PBRShader
 */

#ifndef __AX__ASSET__PBR_MATERIAL_H
#define __AX__ASSET__PBR_MATERIAL_H

#include "AXengine/Asset/Material.h"

namespace AX { namespace Asset {

class PBRMaterial : public Material {
public:
	PBRMaterial();

	TextureMap<glm::vec3> normalMap;
	TextureMap<Tool::F32> metallicMap;
	TextureMap<Tool::F32> roughnessMap;
	TextureMap<Tool::F32> aoMap;

};

} } // namespace AX::Asset

#endif // __AX__ASSET__PBR_MATERIAL_H
