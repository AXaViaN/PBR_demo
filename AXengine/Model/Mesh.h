/**
 *	File: AXengine/Model/Mesh.h
 *	Purpose: Provide basic model data for renderer
 */

#ifndef __AX__MODEL__MESH_H
#define __AX__MODEL__MESH_H

#include "AXengine/Tool/Utility.h"
#include <vector>

namespace AX { namespace Model {

enum VBOlayout {
	POSITION,
	UVCOORD,
	NORMAL,

	LAYOUT_SIZE
};

class Mesh {
public:
	Mesh() : _vaoID(0), _vertexCount(0) { }
	/**
	 * Call AX::Tool::Loader::LoadMesh to create a Mesh
	 */
	Mesh(Tool::U32 vaoID, std::vector<Tool::U32> vboIDList, Tool::SIZE vertexCount) : _vboIDList(vboIDList), _vaoID(vaoID), _vertexCount(vertexCount) { }

	/**
	 * Must be called in order to free memory
	 * 
	 * It is safe to call even if it's disposed before
	 */
	void Dispose();

	const Tool::U32& GetVaoID() const { return _vaoID; }
	const Tool::SIZE& GetVboCount() const { return _vboIDList.size(); }
	const Tool::SIZE& GetVertexCount() const { return _vertexCount; }

private:
	std::vector<Tool::U32> _vboIDList;

	Tool::U32 _vaoID;
	Tool::SIZE _vertexCount;

};

} } // namespace AX::Model

#endif // __AX__MODEL__MESH_H
