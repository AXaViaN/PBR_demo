/**
 *	File: AXengine/Model/Material.h
 *	Purpose: Keeps the necessary information for rendering of models
 */

#ifndef __AX__MODEL__MATERIAL_H
#define __AX__MODEL__MATERIAL_H

#include <glm/glm.hpp>

namespace AX {
namespace Shader { class ShaderProgram; }
namespace Model { class Texture; }
}

namespace AX { namespace Model {

class Material {
public:
	template<typename ValueType>struct TextureMap {
		Model::Texture* texture = nullptr;
		ValueType value;
	};

public:
	Material();
	explicit Material(Shader::ShaderProgram* shader) : shader(shader) { }
	explicit Material(Shader::ShaderProgram& shader) : shader(&shader) { }

	TextureMap<glm::vec3> diffuseMap;

	Shader::ShaderProgram* shader;

};

} } // namespace AX::Model

#endif // __AX__MODEL__MATERIAL_H
