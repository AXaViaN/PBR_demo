#include "AXengine/Asset/PhongMaterial.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

PhongMaterial::PhongMaterial() : Material(&Core::Engine::Instance().phongShader)
{

}

} } // namespace AX::Asset
