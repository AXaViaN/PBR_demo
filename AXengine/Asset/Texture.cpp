#include "AXengine/Asset/Texture.h"

#include <GL/glew.h>

namespace AX { namespace Asset {

void Texture::Dispose()
{
	if(_textureID)
	{
		glDeleteTextures(1, &_textureID);
		_textureID = 0;
	}
}

} } // namespace AX::Asset
