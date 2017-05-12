#include "AXengine/Gfx/FrameBuffer.h"

#include "AXengine/Core/Window.h"
#include <GL/glew.h>

namespace AX { namespace Gfx {

bool FrameBuffer::Init(glm::ivec2 frameSize, Tool::U32 attachmentFlags)
{
	_frameSize = frameSize;
	// 1x1 mipmap level. It is used by GetAvarageBrightness()
	_maxMipmapLevel = glm::floor(glm::log2(glm::max<Tool::F32>(_frameSize.x, _frameSize.y)));

	glGenFramebuffers(1, &_fboID);
	Use();

	attachBuffer((attachmentFlags & COLOR_BUFFER) | (attachmentFlags & HDR_COLOR));
	attachBuffer(attachmentFlags & DEPTH_BUFFER);
	attachBuffer(attachmentFlags & STENCIL_BUFFER);
	attachBuffer(attachmentFlags & DEPTH_STENCIL_BUFFER);

	_textureList[0] = attachTexture((attachmentFlags & COLOR_TEXTURE) | (attachmentFlags & HDR_COLOR));
	_textureList[1] = attachTexture(attachmentFlags & DEPTH_TEXTURE);
	_textureList[2] = attachTexture(attachmentFlags & STENCIL_TEXTURE);
	_textureList[3] = attachTexture(attachmentFlags & DEPTH_STENCIL_TEXTURE);
	
	bool isFBOcomplete = true;
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		isFBOcomplete = false;

	UseDefault();

	return isFBOcomplete;
}
void FrameBuffer::Terminate()
{
	if(_bufferList.size() != 0)
		glDeleteRenderbuffers(_bufferList.size(), &_bufferList[0]);

	for( auto& texture : _textureList)
		texture.Dispose();

	if(_fboID != 0)
		glDeleteFramebuffers(1, &_fboID);
}

void FrameBuffer::Use() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
	glViewport(0, 0, _frameSize.x, _frameSize.y);
}
void FrameBuffer::UseDefault()
{
	glm::ivec2 windowSize = Core::Window::GetWindowSize();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowSize.x, windowSize.y);
}

void FrameBuffer::SetColorTexture(Tool::U32 textureID) const
{
	SetColorTexture(textureID, GL_TEXTURE_2D);
}
void FrameBuffer::SetColorTexture(Tool::U32 textureID, Tool::U32 textureTarget, Tool::U32 mipmapLevel) const
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, textureID, mipmapLevel);
}

Tool::F32 FrameBuffer::GetAvarageBrightness() const
{
	if(_textureList[0].GetTextureID() == 0)
		return 0;

	glBindTexture(GL_TEXTURE_2D, _textureList[0].GetTextureID());

	glGenerateMipmap(GL_TEXTURE_2D);
	Tool::F32 rgb[3];
	glGetTexImage(GL_TEXTURE_2D, _maxMipmapLevel, GL_RGB, GL_FLOAT, &rgb);

	return 0.2126f*rgb[0] + 0.7152f*rgb[1] + 0.0722f*rgb[2];
}

/***** PRIVATE *****/

void FrameBuffer::attachBuffer(Tool::U32 attachmentType)
{
	Tool::U32 bufferFormat;
	Tool::U32 attachmentFormat;
	switch(attachmentType)
	{
		case COLOR_BUFFER: case HDR_COLOR|COLOR_BUFFER:
		{
			if(attachmentType & HDR_COLOR)
				bufferFormat = GL_RGBA16F;
			else
				bufferFormat = GL_RGBA;

			attachmentFormat = GL_COLOR_ATTACHMENT0;

			break;
		}
		case DEPTH_BUFFER:
		{
			bufferFormat = GL_DEPTH_COMPONENT;
			attachmentFormat = GL_DEPTH_ATTACHMENT;

			break;
		}
		case STENCIL_BUFFER:
		{
			bufferFormat = GL_STENCIL_INDEX;
			attachmentFormat = GL_STENCIL_ATTACHMENT;

			break;
		}
		case DEPTH_STENCIL_BUFFER:
		{
			bufferFormat = GL_DEPTH24_STENCIL8;
			attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

			break;
		}

		default:
			return;
	}

	Tool::U32 bufferID;
	glGenRenderbuffers(1, &bufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, bufferID);
	
	glRenderbufferStorage(GL_RENDERBUFFER, bufferFormat, _frameSize.x, _frameSize.y);

	_bufferList.push_back(bufferID);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentFormat, GL_RENDERBUFFER, bufferID);
}
Asset::Texture FrameBuffer::attachTexture(Tool::U32 attachmentType)
{
	Tool::U32 textureDataType = GL_UNSIGNED_BYTE;
	Tool::U32 internalTextureFormat;
	Tool::U32 externalTextureFormat;
	Tool::U32 attachmentFormat;
	switch(attachmentType)
	{
		case COLOR_TEXTURE: case HDR_COLOR|COLOR_TEXTURE:
		{
			if(attachmentType & HDR_COLOR)
				internalTextureFormat = GL_RGBA16F;
			else
				internalTextureFormat = GL_RGBA;

			externalTextureFormat = GL_RGBA;
			attachmentFormat = GL_COLOR_ATTACHMENT0;

			break;
		}
		case DEPTH_TEXTURE:
		{
			internalTextureFormat = GL_DEPTH_COMPONENT;
			externalTextureFormat = GL_DEPTH_COMPONENT;
			attachmentFormat = GL_DEPTH_ATTACHMENT;

			break;
		}
		case STENCIL_TEXTURE:
		{
			internalTextureFormat = GL_STENCIL_INDEX;
			externalTextureFormat = GL_STENCIL_INDEX;
			attachmentFormat = GL_STENCIL_ATTACHMENT;

			break;
		}
		case DEPTH_STENCIL_TEXTURE:
		{
			textureDataType = GL_UNSIGNED_INT_24_8;
			internalTextureFormat = GL_DEPTH24_STENCIL8;
			externalTextureFormat = GL_DEPTH_STENCIL;
			attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

			break;
		}

		default:
			return Asset::Texture(0);
	}

	Tool::U32 textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalTextureFormat, _frameSize.x, _frameSize.y, 0, externalTextureFormat, textureDataType, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentFormat, GL_TEXTURE_2D, textureID, 0);

	return Asset::Texture(textureID);
}

} } // namespace AX::Gfx
