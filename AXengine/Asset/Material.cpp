#include "AXengine/Asset/Material.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

Material::Material() : shader(&Core::Engine::Instance().standardShader)
{
	diffuseMap.value = glm::vec3(0.5, 0.5, 0.5);
}

} } // namespace AX::Asset
