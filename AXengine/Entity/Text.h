/**
 *	File: AXengine/Entity/Text.h
 *	Purpose: Represent a text
 */

#ifndef __AX__ENTITY__TEXT_H
#define __AX__ENTITY__TEXT_H

#include "AXengine/Gfx/TextRenderer.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <string>

namespace AX { namespace Entity {

class Text {
public:
	std::string text;
	Tool::F32 size = 16;

	glm::vec2 position;
	glm::vec3 color;

	glm::vec2 backgroundBorder = glm::vec2(1, 1);
	glm::vec4 backgroundColor = glm::vec4(-1);

	void Render()
	{
		Gfx::TextRenderer::RegisterToBatch(this);
	}

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__TEXT_H
