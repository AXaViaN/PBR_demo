#include "AXengine/Entity/Cubemap.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Entity/EnvironmentProbe.h"
#include "AXengine/Tool/Loader.h"

namespace AX { namespace Entity {

Asset::Mesh Cubemap::mesh;

Cubemap::Cubemap()
{
	material.shader = &Core::Engine::Instance().skyboxShader;
}
Cubemap::Cubemap(EnvironmentProbe* probe)
{
	material.shader = &Core::Engine::Instance().skyboxShader;

	if(probe != nullptr)
		material.diffuseMap.texture = probe->GetEnvironmentMap().material.diffuseMap.texture;
}

void Cubemap::Load(const Tool::CHR* hdrEquirectangularPath)
{
	_texture = Tool::Loader::LoadHDREquirectangular(hdrEquirectangularPath);

	material.diffuseMap.texture = &_texture;
	material.shader = &Core::Engine::Instance().equirectangularShader;
}
void Cubemap::Load(const Tool::CHR* right, const Tool::CHR* left, const Tool::CHR* up, const Tool::CHR* bottom, const Tool::CHR* back, const Tool::CHR* front)
{
	_texture = Tool::Loader::LoadCubeMapTexture({
		right, left, up, bottom, back, front
	});

	material.diffuseMap.texture = &_texture;
}

/***** PROTECTED *****/

void Cubemap::InitMesh()
{
	Tool::F32 positionList[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	mesh = Tool::Loader::LoadMesh(positionList, sizeof(positionList), nullptr, 0);
}

} } // namespace AX::Entity
