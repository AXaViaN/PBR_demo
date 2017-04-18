#include "AXengine/Asset/PBRMaterial.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

PBRMaterial::PBRMaterial() : Material(&Core::Engine::Instance().pbrShader)
{
	normalMap.value = glm::vec3(-1.0f, -1.0f, -1.0f);
	aoMap.value = 0.5f;
}

} } // namespace AX::Asset
