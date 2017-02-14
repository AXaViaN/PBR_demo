#include "AXengine/Model/Texture.h"

#include <GL/glew.h>

namespace AX { namespace Model {

void Texture::Dispose()
{
	if(_textureID)
	{
		glDeleteTextures(1, &_textureID);
		_textureID = 0;
	}
}

} } // namespace AX::Model
