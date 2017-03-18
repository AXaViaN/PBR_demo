#include "AXengine/Asset/PhongMaterial.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

PhongMaterial::PhongMaterial() : Material(&Core::Engine::Instance().phongShader)
{
	normalMap.value = glm::vec3(-1.0f, -1.0f, -1.0f);
	shininess = 128.0f;
}

} } // namespace AX::Asset
