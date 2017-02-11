#include "AXengine/Tool/Loader.h"

#include <vector>
#include <GL/glew.h>

namespace AX { namespace Tool {

Model::Mesh Loader::LoadMesh(F32 positionList[], U32 positionListSize, U32 indexList[], U32 indexListSize)
{
	U32 vaoID;
	glGenVertexArrays(1, &vaoID);

	U32 vboID;
	std::vector<U32> vboIDList;
	vboIDList.reserve(VBOlayout::LAYOUT_SIZE);

	glBindVertexArray(vaoID);
	vboID = storeInVBO(VBOlayout::POSITION, positionList, positionListSize);
	vboIDList.push_back(vboID);

	vboID = bindIndexBuffer(indexList, indexListSize);
	vboIDList.push_back(vboID);

	return Model::Mesh(vaoID, vboIDList, indexListSize);
}

/***** PRIVATE *****/

U32 Loader::storeInVBO(U32 attributeNumber, F32 data[], U32 dataSize)
{
	U32 vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, dataSize*sizeof(F32), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	return vboID;
}
U32 Loader::bindIndexBuffer(U32 indexList[], U32 indexListSize)
{
	U32 vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexListSize*sizeof(U32), &indexList[0], GL_STATIC_DRAW);
	
	return vboID;
}

} } // namespace AX::Tool
