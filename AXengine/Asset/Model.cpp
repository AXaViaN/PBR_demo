#include "AXengine/Asset/Model.h"

#include "AXengine/Asset/PhongMaterial.h"
#include "AXengine/Tool/Loader.h"

namespace AX { namespace Asset {

template class Model<PhongMaterial>;

void Model<PhongMaterial>::Load(const Tool::CHR* filePath)
{
	*this = Tool::Loader::LoadPhongModel(filePath);
}

template<typename T> Entity::GameObject* Model<T>::InstantiateGameObject()
{
	_instantiatedList.push_back(modelParts);

	for( auto& model : _instantiatedList.back() )
		model.RemoveChildren();

	reconnectChildren(&modelParts[0], &_instantiatedList.back()[0]);

	return &_instantiatedList.back()[0];
}

/***** PRIVATE *****/

template<typename T> void Model<T>::reconnectChildren(Entity::GameObject* src, Entity::GameObject* dst)
{
	for( Tool::SIZE i=0 ; i<src->GetChildCount() ; i++ )
	{
		Tool::SIZE childOffset = src->GetChild(i) - src;
		dst->AddChild(dst + childOffset);

		reconnectChildren(src->GetChild(i), dst->GetChild(i));
	}
}

} } // namespace AX::Asset