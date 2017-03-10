/**
 *	File: AXengine/Gfx/TextRenderer.h
 *	Purpose: Handle rendering text to window
 */

#ifndef __AX__GFX__TEXT_RENDERER_H
#define __AX__GFX__TEXT_RENDERER_H

#include "AXengine/Asset/Material.h"
#include "AXengine/Asset/Mesh.h"
#include "AXengine/Asset/Texture.h"
#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <map>

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace AX { namespace Entity {
class Text;
} }

namespace AX { namespace Gfx {

class TextRenderer : public Tool::Singleton<TextRenderer> {
public:
	static bool Init(const Tool::CHR* fontPath);
	static void Terminate();

	static void Render(std::string text, glm::vec3 color, glm::vec2 position, Tool::F32 size);
	static void Render(const Entity::Text& text);

	static void RegisterToBatch(const Entity::Text* text);
	static void RenderBatch();

private:
	struct Character {
		Asset::Texture texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		Tool::I32 advance;
	};

	void initCharacterMap();
	void createTextMesh();

	static void renderBackground(const Entity::Text& text);
	
private:
	FT_LibraryRec_* _fontLibrary;
	FT_FaceRec_* _fontFace;

	Asset::Material _textMaterial;
	Asset::Mesh _textMesh;

	std::map<Tool::CHR, Character> _characterMap;
	std::vector<const Entity::Text*> _textRenderBatch;

	Tool::U32 _meshPositionBuffer;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__TEXT_RENDERER_H
