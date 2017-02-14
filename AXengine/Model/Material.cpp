#include "AXengine/Model/Material.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Model {

Material::Material() : shader(&Core::Engine::Instance().standardShader)
{

}

} } // namespace AX::Model
