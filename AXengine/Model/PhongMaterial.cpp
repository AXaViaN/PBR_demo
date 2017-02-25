#include "AXengine/Model/PhongMaterial.h"

#include "AXengine/Core/Engine.h"

namespace AX { namespace Model {

PhongMaterial::PhongMaterial() : Material(&Core::Engine::Instance().phongShader)
{

}

} } // namespace AX::Model
