#include "AXengine/Asset/Model.h"

#include "AXengine/Asset/PhongMaterial.h"
#include "AXengine/Tool/Loader.h"

namespace AX { namespace Asset {

void Model<PhongMaterial>::Load(const Tool::CHR* filePath)
{
	*this = Tool::Loader::LoadPhongModel(filePath);
}

} } // namespace AX::Asset
