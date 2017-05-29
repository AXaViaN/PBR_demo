/**
 *	File: AXengine/Tool/Loader.h
 *	Purpose: Load all the external graphics data
 */

#ifndef __AX__TOOL__LOADER_H
#define __AX__TOOL__LOADER_H

#include "AXengine/Asset/Mesh.h"
#include "AXengine/Asset/Texture.h"
#include "AXengine/Asset/Model.h"
#include "AXengine/Asset/PhongMaterial.h"
#include "AXengine/Asset/PBRMaterial.h"
#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <assimp/Importer.hpp>
#include <vector>

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Tool {

class Loader : public Singleton<Loader> {
public:
	static Asset::Model<Asset::PhongMaterial> LoadPhongModel(const CHR* filePath, bool useSmoothNormals);
	static Asset::Model<Asset::PBRMaterial> LoadPBRModel(const CHR* filePath, bool useSmoothNormals);

	static Asset::Texture LoadTexture(const CHR* filePath, bool addMipmap, bool isSRGB=true);
	static Asset::Texture LoadCubeMapTexture(std::vector<const CHR*> filePathList);

	static Asset::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								U32 indexList[], SIZE indexListSize);
	static Asset::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);
	static Asset::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								F32 normalList[], SIZE normalListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);
	static Asset::Mesh LoadMesh(F32 positionList[], SIZE positionListSize, 
								F32 normalList[], SIZE normalListSize, 
								F32 tangentList[], SIZE tangentListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);

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
	static Asset::Mesh loadMesh(F32 positionList[], SIZE positionListSize, 
								F32 normalList[], SIZE normalListSize, 
								F32 tangentList[], SIZE tangentListSize, 
								F32 uvCoordList[], SIZE uvCoordListSize, 
								U32 indexList[], SIZE indexListSize);
	static U32 storeInVBO(U32 attributeNumber, U32 dimension, F32 data[], SIZE dataSize);
	static U32 bindIndexBuffer(U32 indexList[], SIZE indexListSize);

	Assimp::Importer _modelImporter;
	
	class Helper;

};

} } // namespace AX::Tool

#endif // __AX__TOOL__LOADER_H
