/**
 *	File: AXengine/Asset/Material.h
 *	Purpose: Keeps the necessary information for rendering of models
 */

#ifndef __AX__ASSET__MATERIAL_H
#define __AX__ASSET__MATERIAL_H

#include <glm/glm.hpp>

namespace AX {
namespace Shader { class ShaderProgram; }
namespace Asset { class Texture; }
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
	explicit Material(Shader::ShaderProgram* shader) : shader(shader) { diffuseMap.value = glm::vec3(0.5, 0.5, 0.5); }
	explicit Material(Shader::ShaderProgram& shader) : shader(&shader) { diffuseMap.value = glm::vec3(0.5, 0.5, 0.5); }

	TextureMap<glm::vec3> diffuseMap;

	Shader::ShaderProgram* shader;

};

} } // namespace AX::Asset

#endif // __AX__ASSET__MATERIAL_H
