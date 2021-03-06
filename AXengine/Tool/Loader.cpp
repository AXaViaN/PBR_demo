#include "AXengine/Tool/Loader.h"

#include "AXengine/Tool/Debug.h"
#include <map>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace AX { namespace Tool {

class Loader::Helper {
public:
	struct ModelConnections {
		std::vector<U32> objectChildMap;
		std::map<U32, U32> objectMeshMap;
		std::map<U32, U32> objectMaterialMap;
		std::map<U32, U32> materialDiffuseMap;
		std::map<U32, U32> materialNormalMap;
	};
	struct PhongModelConnections : public ModelConnections {
		std::map<U32, U32> materialSpecularMap;
		std::map<U32, U32> materialEmissionMap;
		std::map<U32, U32> materialReflectionMap;
	};
	struct PBRModelConnections : public ModelConnections {
		std::map<U32, U32> materialMetallicMap;
		std::map<U32, U32> materialRoughnessMap;
		std::map<U32, U32> materialEmissionMap;
		std::map<U32, U32> materialAOMap;
	};

public:
	static const aiScene* LoadScene(const CHR* filePath, bool useSmoothNormals);

	static void ProcessPhongModel(std::string& directory, const aiScene*& scene, aiNode* currentNode, Asset::Model<Asset::PhongMaterial>& model, U32 currentObjectIndex, PhongModelConnections& modelConnections);
	static void ProcessPBRModel(std::string& directory, const aiScene*& scene, aiNode* currentNode, Asset::Model<Asset::PBRMaterial>& model, U32 currentObjectIndex, PBRModelConnections& modelConnections);
	
	template<typename MaterialType> static void ConnectModel(Asset::Model<MaterialType>& model, ModelConnections& modelConnections);
	static void ConnectPhongModel(Asset::Model<Asset::PhongMaterial>& model, PhongModelConnections& modelConnections);
	static void ConnectPBRModel(Asset::Model<Asset::PBRMaterial>& model, PBRModelConnections& modelConnections);

private:
	static Asset::Mesh getModelMesh(aiMesh* mesh);

};

/***** LOADER *****/

Asset::Model<Asset::PhongMaterial> Loader::LoadPhongModel(const CHR* filePath, bool useSmoothNormals)
{
	Asset::Model<Asset::PhongMaterial> model;

	const aiScene* scene = Helper::LoadScene(filePath, useSmoothNormals);
	if(scene == nullptr)
	{
		Debug::LogWarning("Loading model %s failed!", filePath);
		return model;
	}
	
	// Texture paths are relative to directory
	std::string directory = filePath;
	directory = directory.substr(0, directory.find_last_of('/') + 1);

	Helper::PhongModelConnections modelConnections;
	Helper::ProcessPhongModel(directory, scene, scene->mRootNode, model, 0, modelConnections);
	Helper::ConnectPhongModel(model, modelConnections);
	
	return model;
}
Asset::Model<Asset::PBRMaterial> Loader::LoadPBRModel(const CHR* filePath, bool useSmoothNormals)
{
	Asset::Model<Asset::PBRMaterial> model;

	const aiScene* scene = Helper::LoadScene(filePath, useSmoothNormals);
	if(scene == nullptr)
	{
		Debug::LogWarning("Loading model %s failed!", filePath);
		return model;
	}

	// Texture paths are relative to directory
	std::string directory = filePath;
	directory = directory.substr(0, directory.find_last_of('/') + 1);

	Helper::PBRModelConnections modelConnections;
	Helper::ProcessPBRModel(directory, scene, scene->mRootNode, model, 0, modelConnections);
	Helper::ConnectPBRModel(model, modelConnections);

	return model;
}

Asset::Texture Loader::LoadTexture(const CHR* filePath, bool addMipmap, bool isSRGB)
{
	SDL_Surface* texture = IMG_Load(filePath);
	if(texture == nullptr)
	{
		Debug::LogWarning("Loading texture from %s failed!", filePath);
		return Asset::Texture(0);
	}

	U32 textureID;
	glGenTextures(1, &textureID);
	if(textureID == 0)
	{
		Debug::LogWarning("OpenGL texture generator failed!");
		return Asset::Texture(0);
	}

	// Decide pixel format for texture
	GLenum pixelFormat;
	GLenum internalFormat;
	switch(texture->format->BytesPerPixel)
	{
		case 1:
			pixelFormat = GL_RED;
			internalFormat = GL_RED;
			break;
		case 3:
			if(texture->format->Rmask == 0x000000ff)
				pixelFormat = GL_RGB;
			else
				pixelFormat = GL_BGR;

			if(isSRGB)
				internalFormat = GL_SRGB;
			else
				internalFormat = GL_RGB;
			break;
		case 4:
			if(texture->format->Rmask == 0x000000ff)
				pixelFormat = GL_RGBA;
			else
				pixelFormat = GL_BGRA;

			if(isSRGB)
				internalFormat = GL_SRGB_ALPHA;
			else
				internalFormat = GL_RGBA;
			break;
		default:
			Debug::LogWarning("PixelFormat is not supported for texture %s! BytesPerPixel = %d", filePath, texture->format->BytesPerPixel);

			SDL_FreeSurface(texture);
			glDeleteTextures(1, &textureID);
			return Asset::Texture();
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->w, texture->h, 0, pixelFormat, GL_UNSIGNED_BYTE, texture->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

	return Asset::Texture(textureID);
}
Asset::Texture Loader::LoadCubeMapTexture(std::vector<const CHR*> filePathList)
{
	U32 textureID;
	glGenTextures(1, &textureID);
	if(textureID == 0)
	{
		Debug::LogWarning("OpenGL texture generator failed!");
		return Asset::Texture(0);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	SIZE fileCount = filePathList.size();
	if(fileCount > 6)
		fileCount = 6;
	for( SIZE i=0 ; i<fileCount ; i++ )
	{
		SDL_Surface* texture = IMG_Load(filePathList[i]);
		if(texture == nullptr)
		{
			Debug::LogWarning("Loading texture from %s failed!", filePathList[i]);

			glDeleteTextures(1, &textureID);
			return Asset::Texture();
		}

		// Decide pixel format for texture
		GLenum pixelFormat;
		switch(texture->format->BytesPerPixel)
		{
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
				Debug::LogWarning("PixelFormat is not supported for texture %s! BytesPerPixel = %d", filePathList[i], texture->format->BytesPerPixel);

				SDL_FreeSurface(texture);
				glDeleteTextures(1, &textureID);
				return Asset::Texture();
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, texture->w, texture->h, 0, pixelFormat, GL_UNSIGNED_BYTE, texture->pixels);

		SDL_FreeSurface(texture);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return Asset::Texture(textureID);
}
Asset::Texture Loader::LoadHDREquirectangular(const CHR* filePath)
{
	I32 width, height, components;
	F32* pixels = stbi_loadf(filePath, &width, &height, &components, 0);
	if(pixels == nullptr)
	{
		Debug::LogWarning("Loading HDR equirectangular from %s failed!", filePath);
		return Asset::Texture(0);
	}

	U32 textureID;
	glGenTextures(1, &textureID);
	if(textureID == 0)
	{
		Debug::LogWarning("OpenGL texture generator failed!");
		return Asset::Texture(0);
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(pixels);

	return Asset::Texture(textureID);
}

Asset::Mesh Loader::LoadMesh(F32 positionList[], SIZE positionListSize, U32 indexList[], SIZE indexListSize)
{
	return loadMesh(positionList, positionListSize, nullptr, 0, nullptr, 0, nullptr, 0, indexList, indexListSize);
}
Asset::Mesh Loader::LoadMesh(F32 positionList[], SIZE positionListSize, F32 uvCoordList[], SIZE uvCoordListSize, U32 indexList[], SIZE indexListSize)
{
	return loadMesh(positionList, positionListSize, nullptr, 0, nullptr, 0, uvCoordList, uvCoordListSize, indexList, indexListSize);
}
Asset::Mesh Loader::LoadMesh(F32 positionList[], SIZE positionListSize, F32 normalList[], SIZE normalListSize, F32 uvCoordList[], SIZE uvCoordListSize, U32 indexList[], SIZE indexListSize)
{
	return loadMesh(positionList, positionListSize, normalList, normalListSize, nullptr, 0, uvCoordList, uvCoordListSize, indexList, indexListSize);
}
Asset::Mesh Loader::LoadMesh(F32 positionList[], SIZE positionListSize, F32 normalList[], SIZE normalListSize, F32 tangentList[], SIZE tangentListSize, F32 uvCoordList[], SIZE uvCoordListSize, U32 indexList[], SIZE indexListSize)
{
	return loadMesh(positionList, positionListSize, normalList, normalListSize, tangentList, tangentListSize, uvCoordList, uvCoordListSize, indexList, indexListSize);
}

/***** PROTECTED *****/

bool Loader::Init()
{
	Singleton::init();

	U32 imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
	U32 initResult = IMG_Init(imgFlags);
	if((initResult & imgFlags) != imgFlags)
	{
		Debug::LogWarning("Texture loader failed to initialize.");
		return false;
	}

	stbi_set_flip_vertically_on_load(true);

	return true;
}
void Loader::Terminate()
{
	IMG_Quit();
}

/***** PRIVATE *****/

Asset::Mesh Loader::loadMesh(F32 positionList[], SIZE positionListSize, F32 normalList[], SIZE normalListSize, F32 tangentList[], SIZE tangentListSize, F32 uvCoordList[], SIZE uvCoordListSize, U32 indexList[], SIZE indexListSize)
{
	U32 vaoID;
	glGenVertexArrays(1, &vaoID);

	U32 vboID;
	std::vector<U32> vboIDList;
	vboIDList.reserve(Asset::Mesh::VBOlayout::LAYOUT_SIZE);

	glBindVertexArray(vaoID);
	vboID = storeInVBO(Asset::Mesh::VBOlayout::POSITION, 3, positionList, positionListSize);
	vboIDList.push_back(vboID);

	vboID = storeInVBO(Asset::Mesh::VBOlayout::UVCOORD, 2, uvCoordList, uvCoordListSize);
	vboIDList.push_back(vboID);

	vboID = storeInVBO(Asset::Mesh::VBOlayout::NORMAL, 3, normalList, normalListSize);
	vboIDList.push_back(vboID);

	vboID = storeInVBO(Asset::Mesh::VBOlayout::TANGENT, 3, tangentList, tangentListSize);
	vboIDList.push_back(vboID);

	vboID = bindIndexBuffer(indexList, indexListSize);
	vboIDList.push_back(vboID);

	return Asset::Mesh(vaoID, vboIDList, indexListSize / sizeof(U32));
}
U32 Loader::storeInVBO(U32 attributeNumber, U32 dimension, F32 data[], SIZE dataSize)
{
	U32 vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	if(data)
		glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, dimension, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	return vboID;
}
U32 Loader::bindIndexBuffer(U32 indexList[], SIZE indexListSize)
{
	U32 vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexListSize, &indexList[0], GL_STATIC_DRAW);
	
	return vboID;
}

/***** HELPER PUBLIC *****/

const aiScene* Loader::Helper::LoadScene(const CHR* filePath, bool useSmoothNormals)
{
	U32 normalFlag = aiProcess_GenNormals;
	if(useSmoothNormals)
		normalFlag = aiProcess_GenSmoothNormals;

	const aiScene* scene = instance->_modelImporter.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | normalFlag | aiProcess_CalcTangentSpace);
	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		Debug::LogWarning("%s", instance->_modelImporter.GetErrorString());
		return nullptr;
	}

	return scene;
}

void Loader::Helper::ProcessPhongModel(std::string& directory, const aiScene*& scene, aiNode* currentNode, Asset::Model<Asset::PhongMaterial>& model, U32 currentObjectIndex, PhongModelConnections& modelConnections)
{
	model.modelParts[currentObjectIndex].name = currentNode->mName.C_Str();

	for( SIZE i=0 ; i<currentNode->mNumMeshes ; i++ )
	{
		if(i > 0)
		{
			model.modelParts.push_back(Entity::GameObject());
			U32 nextObjectIndex = model.modelParts.size() - 1;
			modelConnections.objectChildMap.push_back(currentObjectIndex);
			modelConnections.objectChildMap.push_back(nextObjectIndex);

			currentObjectIndex = nextObjectIndex;
		}

		aiMesh* mesh = scene->mMeshes[currentNode->mMeshes[i]];
		model.modelParts[currentObjectIndex].name = currentNode->mName.C_Str();
		model.modelParts[currentObjectIndex].name += "::";
		model.modelParts[currentObjectIndex].name += mesh->mName.C_Str();

		// Process mesh
		model.meshList.push_back(getModelMesh(mesh));
		modelConnections.objectMeshMap.insert(std::pair<U32, U32>(currentObjectIndex, model.meshList.size()-1));

		// Process material
		model.materialList.push_back(Asset::PhongMaterial());
		U32 currentMaterialIndex = model.materialList.size() - 1;

		if(mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString texturePath;

			if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialDiffuseMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].diffuseMap.value = glm::vec4(color.r, color.g, color.b, color.a);
			}

			if(material->GetTextureCount(aiTextureType_SPECULAR) > 0)
			{
				material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialSpecularMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].specularMap.value = glm::vec3(color.r, color.g, color.b);
			}

			F32 shininess;
			if(aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess) == AI_SUCCESS)
				model.materialList[currentMaterialIndex].shininess = shininess;

			if(material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
			{
				material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialEmissionMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].emissionMap.value = glm::vec3(color.r, color.g, color.b);
			}

			// Reflection Maps loaded as ambient map
			if(material->GetTextureCount(aiTextureType_AMBIENT) > 0)
			{
				material->GetTexture(aiTextureType_AMBIENT, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialReflectionMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].reflectionMap.value = color.r;
			}

			// Normal Maps loaded as height map
			if(material->GetTextureCount(aiTextureType_HEIGHT) > 0)
			{
				material->GetTexture(aiTextureType_HEIGHT, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true, false));
				modelConnections.materialNormalMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
		}

		modelConnections.objectMaterialMap.insert(std::pair<U32, U32>(currentObjectIndex, currentMaterialIndex));
	}

	for( SIZE i=0 ; i<currentNode->mNumChildren ; i++ )
	{
		model.modelParts.push_back(Entity::GameObject());
		U32 nextObjectIndex = model.modelParts.size() - 1;
		modelConnections.objectChildMap.push_back(currentObjectIndex);
		modelConnections.objectChildMap.push_back(nextObjectIndex);

		ProcessPhongModel(directory, scene, currentNode->mChildren[i], model, nextObjectIndex, modelConnections);
	}
}
void Loader::Helper::ProcessPBRModel(std::string& directory, const aiScene*& scene, aiNode* currentNode, Asset::Model<Asset::PBRMaterial>& model, U32 currentObjectIndex, PBRModelConnections& modelConnections)
{
	model.modelParts[currentObjectIndex].name = currentNode->mName.C_Str();

	for( SIZE i=0 ; i<currentNode->mNumMeshes ; i++ )
	{
		if(i > 0)
		{
			model.modelParts.push_back(Entity::GameObject());
			U32 nextObjectIndex = model.modelParts.size() - 1;
			modelConnections.objectChildMap.push_back(currentObjectIndex);
			modelConnections.objectChildMap.push_back(nextObjectIndex);

			currentObjectIndex = nextObjectIndex;
		}

		aiMesh* mesh = scene->mMeshes[currentNode->mMeshes[i]];
		model.modelParts[currentObjectIndex].name = currentNode->mName.C_Str();
		model.modelParts[currentObjectIndex].name += "::";
		model.modelParts[currentObjectIndex].name += mesh->mName.C_Str();

		// Process mesh
		model.meshList.push_back(getModelMesh(mesh));
		modelConnections.objectMeshMap.insert(std::pair<U32, U32>(currentObjectIndex, model.meshList.size()-1));

		// Process material
		model.materialList.push_back(Asset::PBRMaterial());
		U32 currentMaterialIndex = model.materialList.size() - 1;

		if(mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString texturePath;

			if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialDiffuseMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].diffuseMap.value = glm::vec4(color.r, color.g, color.b, color.a);
			}

			// Metallic Maps loaded as Specular map
			if(material->GetTextureCount(aiTextureType_SPECULAR) > 0)
			{
				material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialMetallicMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].metallicMap.value = color.r;
			}

			// Roughness Maps loaded as Shininess map
			if(material->GetTextureCount(aiTextureType_SHININESS) > 0)
			{
				material->GetTexture(aiTextureType_SHININESS, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialRoughnessMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				F32 roughness;
				if(aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &roughness) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].roughnessMap.value = roughness;
			}

			if(material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
			{
				material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialEmissionMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].emissionMap.value = glm::vec3(color.r, color.g, color.b);
			}

			if(material->GetTextureCount(aiTextureType_AMBIENT) > 0)
			{
				material->GetTexture(aiTextureType_AMBIENT, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true));
				modelConnections.materialAOMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
			else
			{
				aiColor4D color;
				if(aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
					model.materialList[currentMaterialIndex].aoMap.value = color.r;
			}

			// Normal Maps loaded as height map
			if(material->GetTextureCount(aiTextureType_HEIGHT) > 0)
			{
				material->GetTexture(aiTextureType_HEIGHT, 0, &texturePath);
				model.textureList.push_back(LoadTexture((directory+texturePath.C_Str()).c_str(), true, false));
				modelConnections.materialNormalMap.insert(std::pair<U32, U32>(currentMaterialIndex, model.textureList.size()-1));
			}
		}

		modelConnections.objectMaterialMap.insert(std::pair<U32, U32>(currentObjectIndex, currentMaterialIndex));
	}

	for( SIZE i=0 ; i<currentNode->mNumChildren ; i++ )
	{
		model.modelParts.push_back(Entity::GameObject());
		U32 nextObjectIndex = model.modelParts.size() - 1;
		modelConnections.objectChildMap.push_back(currentObjectIndex);
		modelConnections.objectChildMap.push_back(nextObjectIndex);

		ProcessPBRModel(directory, scene, currentNode->mChildren[i], model, nextObjectIndex, modelConnections);
	}
}

template<typename MaterialType> void Loader::Helper::ConnectModel(Asset::Model<MaterialType>& model, ModelConnections& modelConnections)
{
	for( SIZE i=0 ; i<modelConnections.objectChildMap.size() ; i+=2 )
		model.modelParts[modelConnections.objectChildMap[i]].AddChild(model.modelParts[modelConnections.objectChildMap[i+1]]);

	for( auto objectMesh : modelConnections.objectMeshMap )
		model.modelParts[objectMesh.first].mesh = &model.meshList[objectMesh.second];
	for( auto objectMaterial : modelConnections.objectMaterialMap )
		model.modelParts[objectMaterial.first].material = &model.materialList[objectMaterial.second];

	for( auto materialDiffuse : modelConnections.materialDiffuseMap )
		model.materialList[materialDiffuse.first].diffuseMap.texture = &model.textureList[materialDiffuse.second];
	for( auto materialNormal : modelConnections.materialNormalMap )
		model.materialList[materialNormal.first].normalMap.texture = &model.textureList[materialNormal.second];
}
void Loader::Helper::ConnectPhongModel(Asset::Model<Asset::PhongMaterial>& model, PhongModelConnections& modelConnections)
{
	ConnectModel(model, modelConnections);

	for( auto materialSpecular : modelConnections.materialSpecularMap )
		model.materialList[materialSpecular.first].specularMap.texture = &model.textureList[materialSpecular.second];
	for( auto materialEmission : modelConnections.materialEmissionMap )
		model.materialList[materialEmission.first].emissionMap.texture = &model.textureList[materialEmission.second];
	for( auto materialReflection : modelConnections.materialReflectionMap )
		model.materialList[materialReflection.first].reflectionMap.texture = &model.textureList[materialReflection.second];
}
void Loader::Helper::ConnectPBRModel(Asset::Model<Asset::PBRMaterial>& model, PBRModelConnections& modelConnections)
{
	ConnectModel(model, modelConnections);
	
	for( auto materialMetallic : modelConnections.materialMetallicMap )
		model.materialList[materialMetallic.first].metallicMap.texture = &model.textureList[materialMetallic.second];
	for( auto materialRoughness : modelConnections.materialRoughnessMap )
		model.materialList[materialRoughness.first].roughnessMap.texture = &model.textureList[materialRoughness.second];
	for( auto materialEmission : modelConnections.materialEmissionMap )
		model.materialList[materialEmission.first].emissionMap.texture = &model.textureList[materialEmission.second];
	for( auto materialAO : modelConnections.materialAOMap )
		model.materialList[materialAO.first].aoMap.texture = &model.textureList[materialAO.second];
}

/***** HELPER PRIVATE *****/

Asset::Mesh Loader::Helper::getModelMesh(aiMesh* mesh)
{
	std::vector<F32> positionList;
	std::vector<F32> normalList;
	std::vector<F32> tangentList;
	std::vector<F32> uvList;
	std::vector<U32> indexList;
	for( SIZE j=0 ; j<mesh->mNumVertices ; j++ )
	{
		positionList.push_back(mesh->mVertices[j].x);
		positionList.push_back(mesh->mVertices[j].y);
		positionList.push_back(mesh->mVertices[j].z);

		normalList.push_back(mesh->mNormals[j].x);
		normalList.push_back(mesh->mNormals[j].y);
		normalList.push_back(mesh->mNormals[j].z);

		if(mesh->mTangents)
		{
			tangentList.push_back(mesh->mTangents[j].x);
			tangentList.push_back(mesh->mTangents[j].y);
			tangentList.push_back(mesh->mTangents[j].z);
		}
		
		if(mesh->mTextureCoords[0])
		{
			while(mesh->mTextureCoords[0][j].x > 1)
				mesh->mTextureCoords[0][j].x--;
			while(mesh->mTextureCoords[0][j].y > 1)
				mesh->mTextureCoords[0][j].y--;
			uvList.push_back(mesh->mTextureCoords[0][j].x);
			uvList.push_back(mesh->mTextureCoords[0][j].y);
		}
	}
	for( SIZE j=0 ; j<mesh->mNumFaces ; j++ )
	{
		aiFace& face = mesh->mFaces[j];
		for( SIZE k=0 ; k<face.mNumIndices ; k++ )
			indexList.push_back(face.mIndices[k]);
	}

	F32* uvListPtr = nullptr;
	SIZE uvListSize = 0;
	if(uvList.size() != 0)
	{
		uvListPtr = &uvList[0];
		uvListSize = uvList.size() * sizeof(F32);
	}

	F32* tangentListPtr = nullptr;
	SIZE tangentListSize = 0;
	if(tangentList.size() != 0)
	{
		tangentListPtr = &tangentList[0];
		tangentListSize = tangentList.size() * sizeof(F32);
	}

	return Loader::LoadMesh(&positionList[0], positionList.size()*sizeof(F32), 
							&normalList[0], normalList.size()*sizeof(F32),
							tangentListPtr, tangentListSize,
							uvListPtr, uvListSize,
							&indexList[0], indexList.size()*sizeof(U32));
}

} } // namespace AX::Tool
