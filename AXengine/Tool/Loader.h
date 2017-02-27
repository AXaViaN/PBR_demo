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

class Loader {
public:
	static Model::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								U32 indexList[], SIZE indexListSize);
	static Model::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);
	static Model::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								F32 normalList[], SIZE normalListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);

	static Model::Texture LoadTexture(const CHR* filePath, bool addMipmap);

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
	static Model::Mesh loadMesh(F32 positionList[], SIZE positionListSize, 
								F32 normalList[], SIZE normalListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);
	static U32 storeInVBO(U32 attributeNumber, U32 dimension, F32 data[], SIZE dataSize);
	static U32 bindIndexBuffer(U32 indexList[], SIZE indexListSize);

};

} } // namespace AX::Tool

#endif // __AX__TOOL__LOADER_H
