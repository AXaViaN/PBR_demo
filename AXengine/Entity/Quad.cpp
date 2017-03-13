#include "AXengine/Entity/Quad.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Tool/Loader.h"

namespace AX { namespace Entity {

Asset::Mesh Quad::mesh;

Quad::Quad()
{
	material.shader = &Core::Engine::Instance().standardShader2D;
}

void Quad::InitMesh()
{
	Tool::F32 positionList[] = {
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};
	Tool::F32 uvList[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};
	Tool::U32 indexList[] = {
		0, 1, 2,
		2, 1, 3
	};

	mesh = Tool::Loader::LoadMesh(positionList, sizeof(positionList), uvList, sizeof(uvList), indexList, sizeof(indexList));
}

} } // namespace AX::Entity
