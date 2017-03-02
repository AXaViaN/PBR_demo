#include "AXengine/Asset/Mesh.h"

#include <GL/glew.h>

namespace AX { namespace Asset {

void Mesh::Dispose()
{
	if(_vboIDList.size() != 0)
	{
		glDeleteBuffers(_vboIDList.size(), &_vboIDList[0]);
		_vboIDList.clear();
	}
	
	if(_vaoID != 0)
	{
		glDeleteVertexArrays(1, &_vaoID);
		_vaoID = 0;
	}
}

} } // namespace AX::Asset
