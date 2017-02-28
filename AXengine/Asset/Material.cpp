#include "AXengine/Asset/Material.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Asset {

Material::Material() : shader(&Core::Engine::Instance().standardShader)
{

}

} } // namespace AX::Asset
