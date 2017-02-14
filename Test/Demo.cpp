#include "AXengine/Game.h"

using namespace AX::Entity;
using namespace AX::Model;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	GameObject quadObject;
	Mesh quadModel;
	Texture crateTexture;
	Material quadMaterial;

	void Start()
	{
		F32 vertexList[] = {
			-0.5f,  0.5f,  0.0f,
			-0.5f, -0.5f,  0.0f,
			 0.5f,  0.5f,  0.0f,
			 0.5f, -0.5f,  0.0f
		};
		F32 uvCoordList[] = {
			0.0, 0.0,
			0.0, 1.0,
			1.0, 0.0,
			1.0, 1.0
		};
		U32 indexList[] = {
			0, 1, 2,
			2, 1, 3
		};

		quadModel = Loader::LoadMesh(vertexList, sizeof(vertexList), uvCoordList, sizeof(uvCoordList), indexList, sizeof(indexList));
		crateTexture = Loader::LoadTexture("Data/crate.png");
		
		quadMaterial.diffuseMap.value = glm::vec3(0.3, 0.6, 0.9);
		
		quadObject = GameObject(quadModel, quadMaterial);
	}
	void Dispose()
	{
		quadModel.Dispose();
		crateTexture.Dispose();
	}
	
	void Update()
	{
		if(Input::GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			Debug::LogInfo("Escaped!");
			Exit();
		}

		if(Input::GetKeyDown(SDL_SCANCODE_SPACE))
			quadMaterial.diffuseMap.texture = &crateTexture;
		if(Input::GetKeyUp(SDL_SCANCODE_SPACE))
			quadMaterial.diffuseMap.texture = nullptr;
	}
	
	void Draw()
	{
		quadObject.Render();
	}
	
};

RegisterGameClass(Demo);
