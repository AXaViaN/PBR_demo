/**
 *	File: AXengine/Gfx/FrameBuffer.h
 *	Purpose: Provide off-screen rendering
 */

#ifndef __AX__GFX__FRAME_BUFFER_H
#define __AX__GFX__FRAME_BUFFER_H

#include "AXengine/Asset/Texture.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <vector>

namespace AX { namespace Gfx {

class FrameBuffer {
public:
	enum AttachmentType {
		COLOR_BUFFER          = 0x0001,
		DEPTH_BUFFER          = 0x0002,
		STENCIL_BUFFER        = 0x0004,
		DEPTH_STENCIL_BUFFER  = 0x0008,
		
		COLOR_TEXTURE         = 0x0010,
		DEPTH_TEXTURE         = 0x0020,
		STENCIL_TEXTURE       = 0x0040,
		DEPTH_STENCIL_TEXTURE = 0x0080,

		HDR_COLOR             = 0x0100,
		
		COLOR_TEXTURE__DEPTH_STENCIL_BUFFER = 0x0018,
		COLOR_DEPTH_TEXTURE__STENCIL_BUFFER = 0x0034
	};

public:
	FrameBuffer() : _fboID(0) { }

	bool Init(glm::ivec2 frameSize, Tool::U32 attachmentFlags);
	void Terminate();

	void Use();
	static void UseDefault();

	Tool::F32 GetAvarageBrightness();

	Asset::Texture& GetColorTexture() { return _textureList[0]; }
	Asset::Texture& GetDepthTexture() { return _textureList[1]; }
	Asset::Texture& GetStencilTexture() { return _textureList[2]; }
	Asset::Texture& GetDepthStencilTexture() { return _textureList[3]; }

private:
	void attachBuffer(Tool::U32 attachmentType);
	Asset::Texture attachTexture(Tool::U32 attachmentType);

private:
	std::vector<Tool::U32> _bufferList;
	Asset::Texture _textureList[4];
	
	glm::ivec2 _frameSize;

	Tool::U32 _fboID;
	Tool::I32 _maxMipmapLevel;

};

} } // namespace AX::Gfx

#endif // __AX__GFX__FRAME_BUFFER_H
