/**
 *	File: AXengine/Tool/Loader.h
 *	Purpose: Load all the external graphics data
 */

#ifndef __AX__TOOL__LOADER_H
#define __AX__TOOL__LOADER_H

#include "AXengine/Model/Mesh.h"

namespace AX { namespace Tool {

enum VBOlayout {
	POSITION,

	LAYOUT_SIZE
};

class Loader {
public:
	static Model::Mesh LoadMesh(F32 positionList[], U32 positionListSize, 
								U32 indexList[], U32 indexListSize);

private:
	static U32 storeInVBO(U32 attributeNumber, F32 data[], U32 dataSize);
	static U32 bindIndexBuffer(U32 indexList[], U32 indexListSize);

};

} } // namespace AX::Tool

#endif // __AX__TOOL__LOADER_H
