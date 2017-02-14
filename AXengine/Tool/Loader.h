/**
 *	File: AXengine/Tool/Loader.h
 *	Purpose: Load all the external graphics data
 */

#ifndef __AX__TOOL__LOADER_H
#define __AX__TOOL__LOADER_H

#include "AXengine/Model/Mesh.h"
#include "AXengine/Model/Texture.h"
#include "AXengine/Tool/Utility.h"

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Tool {

enum VBOlayout {
	POSITION,
	UVCOORD,

	LAYOUT_SIZE
};

class Loader {
public:
	static Model::Mesh LoadMesh(F32 positionList[], U32 positionListSize, 
								U32 indexList[], U32 indexListSize);
	static Model::Mesh LoadMesh(F32 positionList[], U32 positionListSize, 
								F32 uvCoordList[], U32 uvCoordListSize, 
								U32 indexList[], U32 indexListSize);

	static Model::Texture LoadTexture(const CHR* filePath);

protected:
	/**
	 * Init and Terminate is only visible for Engine
	 * 
	 * This is done to prevent API users to call these methods and
	 * get UB.
	 */
	friend class Core::Engine;
	static bool Init();
	static void Terminate();

private:
	static Model::Mesh loadMesh(F32 positionList[], U32 positionListSize, 
								F32 uvCoordList[], U32 uvCoordListSize, 
								U32 indexList[], U32 indexListSize);
	static U32 storeInVBO(U32 attributeNumber, U32 dimension, F32 data[], U32 dataSize);
	static U32 bindIndexBuffer(U32 indexList[], U32 indexListSize);

};

} } // namespace AX::Tool

#endif // __AX__TOOL__LOADER_H
