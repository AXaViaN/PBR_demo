/**
 *	File: AXengine/Asset/Model.h
 *	Purpose: Provide complex model data
 */

#ifndef __AX__ASSET__MODEL_H
#define __AX__ASSET__MODEL_H

#include "AXengine/Entity/GameObject.h"
#include "AXengine/Asset/Texture.h"
#include "AXengine/Tool/Utility.h"
#include <vector>

namespace AX { namespace Tool {
class Loader;
} }

namespace AX { namespace Asset {

template<typename MaterialType> class Model {
public:
	Model() { modelParts.push_back(Entity::GameObject()); }

	Entity::GameObject* InstantiateGameObject();

	void Load(const Tool::CHR* filePath, bool useSmoothNormals=false);

	void Dispose()
	{
		for( Mesh& mesh : meshList )
			mesh.Dispose();
		meshList.clear();

		for( Texture& texture : textureList )
			texture.Dispose();
		textureList.clear();

		materialList.clear();
		modelParts.clear();
		modelParts.push_back(Entity::GameObject());
	}
	
protected:
	friend class Tool::Loader;
	
	std::vector<Entity::GameObject> modelParts;
	std::vector<MaterialType> materialList;
	std::vector<Mesh> meshList;
	std::vector<Texture> textureList;

private:
	void reconnectChildren(Entity::GameObject* src, Entity::GameObject* dst);

	std::vector<std::vector<Entity::GameObject>> _instantiatedList;
	
};

} } // namespace AX::Asset

#endif // __AX__ASSET__MODEL_H
