/**
 *	File: AXengine/Entity/Cubemap.h
 *	Purpose: Represent a 3D cubemap
 */

#ifndef __AX__ENTITY__CUBEMAP_H
#define __AX__ENTITY__CUBEMAP_H

#include "AXengine/Asset/Material.h"
#include "AXengine/Asset/Mesh.h"
#include "AXengine/Asset/Texture.h"
#include "AXengine/Tool/Utility.h"

namespace AX {
namespace Core { class Engine; }
namespace Entity { class EnvironmentProbe; }
}

namespace AX { namespace Entity {

class Cubemap {
public:
	Cubemap();
	Cubemap(EnvironmentProbe* probe);

	void Load(const Tool::CHR* hdrEquirectangularPath);
	void Load(const Tool::CHR* right, const Tool::CHR* left, const Tool::CHR* up,
			  const Tool::CHR* bottom, const Tool::CHR* back, const Tool::CHR* front);
	void Dispose()
	{
		_texture.Dispose();
	}

	Asset::Material material;
	static Asset::Mesh mesh;

protected:
	friend class Core::Engine;
	static void InitMesh();
	static void DisposeMesh()
	{
		mesh.Dispose();
	}

private:
	Asset::Texture _texture;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__CUBEMAP_H
