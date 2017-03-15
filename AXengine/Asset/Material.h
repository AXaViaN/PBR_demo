/**
 *	File: AXengine/Asset/Material.h
 *	Purpose: Keeps the necessary information for rendering of models
 */

#ifndef __AX__ASSET__MATERIAL_H
#define __AX__ASSET__MATERIAL_H

#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX {
namespace Shader { class ShaderProgram; }
namespace Asset { class Texture; }
namespace Entity { class Cubemap; }
}

namespace AX { namespace Asset {

class Material {
public:
	template<typename ValueType>struct TextureMap {
		Texture* texture = nullptr;
		ValueType value;
	};

public:
	Material();
	explicit Material(Shader::ShaderProgram* shader) : shader(shader) { setDefaults(); }
	explicit Material(Shader::ShaderProgram& shader) : shader(&shader) { setDefaults(); }

	TextureMap<glm::vec4> diffuseMap;
	TextureMap<Tool::F32> reflectionMap;

	Entity::Cubemap* environmentMap;

	Shader::ShaderProgram* shader;

private:
	void setDefaults();

};

} } // namespace AX::Asset

#endif // __AX__ASSET__MATERIAL_H
