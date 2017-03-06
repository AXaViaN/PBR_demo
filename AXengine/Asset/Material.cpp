#include "AXengine/Asset/Material.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

Material::Material() : shader(&Core::Engine::Instance().standardShader)
{
	diffuseMap.value = glm::vec4(0.5, 0.5, 0.5, 1.0);
}

} } // namespace AX::Asset
