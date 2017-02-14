/**
 *	File: AXengine/Model/Texture.h
 *	Purpose: Texture info for models
 */

#ifndef __AX__MODEL__TEXTURE_H
#define __AX__MODEL__TEXTURE_H

#include "AXengine/Tool/Utility.h"

namespace AX { namespace Model {

class Texture {
public:
	Texture() : _textureID(0) { }
	explicit Texture(Tool::U32 textureID) : _textureID(textureID) { }
	
	/**
	 * Must be called in order to free memory
	 * 
	 * It is safe to call even if it's disposed before
	 */
	void Dispose();

	const Tool::U32& GetTextureID() const { return _textureID; }

private:
	Tool::U32 _textureID;

};

} } // namespace AX::Model

#endif // __AX__MODEL__TEXTURE_H
