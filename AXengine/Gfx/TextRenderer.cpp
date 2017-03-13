#include "AXengine/Gfx/TextRenderer.h"
#include "AXengine/Gfx/Renderer.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Entity/Text.h"
#include "AXengine/Tool/Debug.h"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace AX { namespace Gfx {

static const Tool::SIZE LOADED_FONT_SIZE = 128;

bool TextRenderer::Init(const Tool::CHR* fontPath)
{
	Tool::Singleton<TextRenderer>::init();

	Tool::U32 initResult = FT_Init_FreeType(&instance->_fontLibrary);
	if(initResult != 0)
	{
		Tool::Debug::LogWarning("FreeType Library initialization failed: 0x%x", initResult);
		return false;
	}

	initResult = FT_New_Face(instance->_fontLibrary, fontPath, 0, &instance->_fontFace);
	if(initResult != 0)
	{
		Tool::Debug::LogWarning("Font %s cannot be loaded: 0x%x", fontPath, initResult);
		return false;
	}

	FT_Set_Pixel_Sizes(instance->_fontFace, 0, LOADED_FONT_SIZE);
	instance->initCharacterMap();

	FT_Done_Face(instance->_fontFace);
	FT_Done_FreeType(instance->_fontLibrary);

	instance->_textMaterial.shader = &Core::Engine::Instance().textShader;
	instance->createTextMesh();

	return true;
}
void TextRenderer::Terminate()
{
	for( auto& character : instance->_characterMap )
		character.second.texture.Dispose();

	instance->_textMesh.Dispose();

	instance->_characterMap.clear();
}

void TextRenderer::Render(std::string text, glm::vec3 color, glm::vec2 position, Tool::F32 size)
{
	instance->_textMaterial.diffuseMap.value = glm::vec4(color, 1.0f);
	size = size / LOADED_FONT_SIZE;

	instance->_textMaterial.shader->Start();
	
	for( auto& chr : text )
	{
		Character& character = instance->_characterMap['?'];
		if(instance->_characterMap.find(chr) != instance->_characterMap.end())
			character = instance->_characterMap[chr];

		Tool::F32 x = position.x + character.bearing.x * size;
		Tool::F32 y = position.y + character.bearing.y * size;

		Tool::F32 w = character.size.x * size;
		Tool::F32 h = character.size.y * size;

        // Update buffer for each character
		Tool::F32 positionList[] = {
			x, y,
			x, y-h,
			x+w, y,
			x+w, y-h
		};
		glBindBuffer(GL_ARRAY_BUFFER, instance->_meshPositionBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionList), positionList);

		instance->_textMaterial.diffuseMap.texture = &character.texture;
		instance->_textMaterial.shader->ProcessMaterial(instance->_textMaterial);
		Renderer::Render(instance->_textMesh);

		// Advance cursor for next glyph
		position.x += character.advance * size;
	}
	
	instance->_textMaterial.shader->Stop();
}
void TextRenderer::Render(const Entity::Text& text)
{
	if(text.backgroundColor.r != -1)
		renderBackground(text);

	Render(text.text, text.color, text.position, text.size);
}

void TextRenderer::RegisterToBatch(const Entity::Text* text)
{
	if(text)
		instance->_textRenderBatch.push_back(text);
}
void TextRenderer::RenderBatch()
{
	if(instance==nullptr || instance->_characterMap.size()==0)
		return;

	bool isRendererDebugMode = Renderer::GetDebugMode();
	Renderer::SetDebugMode(false);

	for( auto& text : instance->_textRenderBatch )
		Render(*text);

	Renderer::SetDebugMode(isRendererDebugMode);

	instance->_textRenderBatch.clear();
}

/***** PRIVATE *****/

void TextRenderer::initCharacterMap()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for( Tool::UCHR chr=0 ; chr<128 ; chr++ )
	{
		// Load character
		Tool::U32 initResult = FT_Load_Char(_fontFace, chr, FT_LOAD_RENDER);
		if(initResult != 0)
		{
			Tool::Debug::LogWarning("Character %c cannot be loaded: 0x%x", chr, initResult);
			continue;
		}

		// Generate texture
		Tool::U32 textureID;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _fontFace->glyph->bitmap.width, _fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, _fontFace->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		_characterMap[chr] = {Asset::Texture(textureID),
			glm::ivec2(_fontFace->glyph->bitmap.width, _fontFace->glyph->bitmap.rows),
			glm::ivec2(_fontFace->glyph->bitmap_left, _fontFace->glyph->bitmap_top),
			_fontFace->glyph->advance.x >> 6};
		// Advance is number of 1/64 pixels
	}
}

void TextRenderer::createTextMesh()
{
	Tool::U32 vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	// Generate position buffer
	glGenBuffers(1, &_meshPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Tool::F32) * 2 * 4, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(Asset::Mesh::VBOlayout::POSITION, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Generate uv buffer
	Tool::F32 uvList[] = {
		0.0f, 0.0f, 
		0.0f, 1.0f, 
		1.0f, 0.0f, 
		1.0f, 1.0f 
	};
	Tool::U32 uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvList), &uvList[0], GL_STATIC_DRAW);
	glVertexAttribPointer(Asset::Mesh::VBOlayout::UVCOORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Generate index buffer
	Tool::U32 indexList[] = {
		0, 1, 2,
		2, 1, 3
	};
	Tool::U32 indexBuffer;
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexList), &indexList[0], GL_STATIC_DRAW);

	_textMesh = Asset::Mesh(vaoID, {_meshPositionBuffer, uvBuffer, indexBuffer}, 6);
}

void TextRenderer::renderBackground(const Entity::Text& text)
{
	instance->_textMaterial.shader->Start();

	Tool::F32 size = text.size / LOADED_FONT_SIZE;

	Tool::F32 x = text.position.x - text.backgroundBorder.x;
	Tool::F32 y = text.position.y + text.backgroundBorder.y;

	Tool::F32 w = text.backgroundBorder.x * 2;
	Tool::F32 h = text.backgroundBorder.y * 2;
	for( auto& chr : text.text )
	{
		Character& character = instance->_characterMap['?'];
		if(instance->_characterMap.find(chr) != instance->_characterMap.end())
			character = instance->_characterMap[chr];

		w += character.advance * size;
		y = glm::max<Tool::F32>(y, text.position.y + character.bearing.y * size + text.backgroundBorder.y);
		h = glm::max<Tool::F32>(h, character.size.y * size + text.backgroundBorder.y * 2);
	}

	// Update buffer for each character
	Tool::F32 positionList[] = {
		x, y,
		x, y-h,
		x+w, y,
		x+w, y-h
	};
	glBindBuffer(GL_ARRAY_BUFFER, instance->_meshPositionBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionList), positionList);

	instance->_textMaterial.diffuseMap.texture = nullptr;
	instance->_textMaterial.diffuseMap.value = text.backgroundColor;
	instance->_textMaterial.shader->ProcessMaterial(instance->_textMaterial);
	Renderer::Render(instance->_textMesh);

	instance->_textMaterial.shader->Stop();
}

} } // namespace AX::Gfx
