#include "AXengine/Game.h"

using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Model;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	FreeCamera camera;

	Mesh cubeModel;
	Texture cubeTexture;
	Material cubeMaterial;
	GameObject cubeList[5];

	Mesh surfaceModel;
	Texture surfaceTexture;
	Material surfaceMaterial;
	GameObject surface;

	void Start()
	{
		// Cube
		F32 vertexList[] = {
			-0.5f,0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,-0.5f,
			0.5f,0.5f,-0.5f,

			-0.5f,0.5f,0.5f,
			-0.5f,-0.5f,0.5f,
			0.5f,-0.5f,0.5f,
			0.5f,0.5f,0.5f,

			0.5f,0.5f,-0.5f,
			0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,0.5f,
			0.5f,0.5f,0.5f,

			-0.5f,0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			-0.5f,-0.5f,0.5f,
			-0.5f,0.5f,0.5f,

			-0.5f,0.5f,0.5f,
			-0.5f,0.5f,-0.5f,
			0.5f,0.5f,-0.5f,
			0.5f,0.5f,0.5f,

			-0.5f,-0.5f,0.5f,
			-0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,0.5f
		};
		F32 uvCoordList[] = {
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0
		};
		U32 indexList[] = {
			0,1,3,
			3,1,2,
			4,5,7,
			7,5,6,
			8,9,11,
			11,9,10,
			12,13,15,
			15,13,14,
			16,17,19,
			19,17,18,
			20,21,23,
			23,21,22
		};
		cubeModel = Loader::LoadMesh(vertexList, sizeof(vertexList), uvCoordList, sizeof(uvCoordList), indexList, sizeof(indexList));
		cubeTexture = Loader::LoadTexture("Test/Data/crate.png", true);
		cubeMaterial.diffuseMap.texture = &cubeTexture;
		
		for( GameObject& cube : cubeList )
			cube = GameObject(cubeModel, cubeMaterial);
		cubeList[0].transform.SetPosition(0, 0.5, -3);
		cubeList[1].transform.SetPosition(0, 1.5, -3);
		cubeList[2].transform.SetPosition(2, 0.5, -3);
		cubeList[3].transform.SetPosition(0, 0.5, 3);
		cubeList[4].transform.SetPosition(1, 0.5, 3);
		
		// Surface
		F32 surfaceVertex[] = {
			-1, 0, -1,
			-1, 0,  1,
			 1, 0, -1,
			 1, 0,  1
		};
		F32 surfaceUV[] = {
			0, 0,
			0, 1,
			1, 0,
			1, 1
		};
		U32 surfaceIndex[] = {
			0, 1, 2,
			2, 1, 3
		};
		surfaceModel = Loader::LoadMesh(surfaceVertex, sizeof(surfaceVertex), surfaceUV, sizeof(surfaceUV), surfaceIndex, sizeof(surfaceIndex));
		surfaceTexture = Loader::LoadTexture("Test/Data/dirt.jpg", true);
		surfaceMaterial.diffuseMap.texture = &surfaceTexture;
		surface = GameObject(surfaceModel, surfaceMaterial);
		surface.transform.Scale(5);
		
		camera.transform.Translate(0, 1, 0);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);
		Input::ActivateMouseMotion(false);
	}
	void Dispose()
	{
		cubeModel.Dispose();
		cubeTexture.Dispose();

		surfaceModel.Dispose();
		surfaceTexture.Dispose();
	}
	
	void Update()
	{
		if(Input::GetKeyUp(SDL_SCANCODE_ESCAPE))
		{
			Debug::LogInfo("Escaped!");
			Exit();
		}
		if(Input::GetKeyDown(SDL_SCANCODE_TAB))
		{
			static bool isDebugMode = false;
			cubeMaterial.shader->SetDebugDrawMode(isDebugMode=!isDebugMode);
		}

		if(Input::GetMouseButtonDown(Input::MouseButton::LEFT))
		{
			Input::ActivateMouseMotion(true);
		}
		if(Input::GetMouseButtonDown(Input::MouseButton::RIGHT))
		{
			Input::ActivateMouseMotion(false);
		}

		camera.Update();
	}
	
	void Draw()
	{
		surface.Render(camera);

		for( GameObject& cube : cubeList )
			cube.Render(camera);
	}
	
};

RegisterGameClass(Demo);
