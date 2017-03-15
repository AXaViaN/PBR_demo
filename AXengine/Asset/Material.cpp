#include "AXengine/Asset/Material.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

Material::Material() : shader(&Core::Engine::Instance().standardShader)
{
	setDefaults();
}

/***** PRIVATE *****/

void Material::setDefaults()
{
	diffuseMap.value = glm::vec4(0.5, 0.5, 0.5, 1.0);
	reflectionMap.value = 0;

	environmentMap = nullptr;
}

} } // namespace AX::Asset
