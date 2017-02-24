#include "AXengine/Tool/Loader.h"

#include "AXengine/Tool/Debug.h"
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL_image.h>

namespace AX { namespace Tool {

Model::Mesh Loader::LoadMesh(F32 positionList[], U32 positionListSize, U32 indexList[], U32 indexListSize)
{
	return loadMesh(positionList, positionListSize, nullptr, 0, indexList, indexListSize);
}
Model::Mesh Loader::LoadMesh(F32 positionList[], U32 positionListSize, F32 uvCoordList[], U32 uvCoordListSize, U32 indexList[], U32 indexListSize)
{
	return loadMesh(positionList, positionListSize, uvCoordList, uvCoordListSize, indexList, indexListSize);
}

Model::Texture Loader::LoadTexture(const CHR* filePath, bool addMipmap)
{
	SDL_Surface* texture = IMG_Load(filePath);
	if(texture == nullptr)
	{
		Debug::LogWarning("Loading texture from %s failed!", filePath);
		return Model::Texture(0);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	if(textureID == 0)
	{
		Debug::LogWarning("OpenGL texture generator failed!");
		return Model::Texture(0);
	}

	// Decide pixel format for texture
	GLenum pixelFormat;
	switch(texture->format->BytesPerPixel)
	{
		case 1:
			pixelFormat = GL_R;
			break;
		case 3:
			if(texture->format->Rmask == 0x000000ff)
				pixelFormat = GL_RGB;
			else
				pixelFormat = GL_BGR;
			break;
		case 4:
			if(texture->format->Rmask == 0x000000ff)
				pixelFormat = GL_RGBA;
			else
				pixelFormat = GL_BGRA;
			break;
		default:
			Debug::LogWarning("PixelFormat is not supported for texture %s! BytesPerPixel = %d", filePath, texture->format->BytesPerPixel);
			
			SDL_FreeSurface(texture);
			glDeleteTextures(1, &textureID);
			return Model::Texture();
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, texture->format->BytesPerPixel, texture->w, texture->h, 0, pixelFormat, GL_UNSIGNED_BYTE, texture->pixels);
	
	if(addMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(texture);

	return Model::Texture(textureID);
}

/***** PRIVATE *****/

bool Loader::Init()
{
	U32 imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
	U32 initResult = IMG_Init(imgFlags);
	if((initResult & imgFlags) != imgFlags)
	{
		Debug::LogWarning("Texture loader failed to initialize.");
		return false;
	}

	return true;
}
void Loader::Terminate()
{
	IMG_Quit();
}

/***** PRIVATE *****/

Model::Mesh Loader::loadMesh(F32 positionList[], U32 positionListSize, F32 uvCoordList[], U32 uvCoordListSize, U32 indexList[], U32 indexListSize)
{
	U32 vaoID;
	glGenVertexArrays(1, &vaoID);

	U32 vboID;
	std::vector<U32> vboIDList;
	vboIDList.reserve(Model::VBOlayout::LAYOUT_SIZE);

	glBindVertexArray(vaoID);
	vboID = storeInVBO(Model::VBOlayout::POSITION, 3, positionList, positionListSize);
	vboIDList.push_back(vboID);

	vboID = storeInVBO(Model::VBOlayout::UVCOORD, 2, uvCoordList, uvCoordListSize);
	vboIDList.push_back(vboID);

	vboID = bindIndexBuffer(indexList, indexListSize);
	vboIDList.push_back(vboID);

	return Model::Mesh(vaoID, vboIDList, indexListSize);
}
U32 Loader::storeInVBO(U32 attributeNumber, U32 dimension, F32 data[], U32 dataSize)
{
	U32 vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	if(data)
		glBufferData(GL_ARRAY_BUFFER, dataSize*sizeof(F32), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, dimension, GL_FLOAT, GL_FALSE, 0, nullptr);
	
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
