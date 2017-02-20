#include "AXengine/Game.h"

using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Model;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	FreeCamera camera;
	GameObject quadObject;
	Mesh quadModel;
	Texture crateTexture;
	Material quadMaterial;

	GameObject surfaceObject;
	Mesh surfaceModel;
	Material surfaceMaterial;

	void Start()
	{
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

		quadModel = Loader::LoadMesh(vertexList, sizeof(vertexList), uvCoordList, sizeof(uvCoordList), indexList, sizeof(indexList));
		crateTexture = Loader::LoadTexture("Data/crate.png", true);
		
		quadMaterial.diffuseMap.texture = &crateTexture;
		
		quadObject = GameObject(quadModel, quadMaterial);
		quadObject.transform.SetPosition(0, 0, -2);

		F32 surfaceVertex[] = {
			-1, 0, -1,
			-1, 0,  1,
			 1, 0, -1,
			 1, 0,  1
		};
		U32 surfaceIndex[] = {
			0, 1, 2,
			2, 1, 3
		};
		surfaceModel = Loader::LoadMesh(surfaceVertex, sizeof(surfaceVertex), surfaceIndex, sizeof(surfaceIndex));
		surfaceMaterial.diffuseMap.value = glm::vec3(.5, .3, .1);
		surfaceObject = GameObject(surfaceModel, surfaceMaterial);
		surfaceObject.transform.Scale(100);

		camera.transform.Translate(0, 1, 0);
		camera.SetMovementSpeed(2.5);
		camera.SetRotationSpeed(1.5);
		Input::ActivateMouseMotion(false);
	}
	void Dispose()
	{
		quadModel.Dispose();
		crateTexture.Dispose();

		surfaceModel.Dispose();
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
			quadMaterial.shader->SetDebugDrawMode(isDebugMode=!isDebugMode);
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
		Renderer::Clear(.2, .4, .7);
		surfaceObject.Render(camera);

		quadObject.transform.SetPosition(0, 0.5, -3);
		quadObject.Render(camera);

		quadObject.transform.SetPosition(0, 1.5, -3);
		quadObject.Render(camera);

		quadObject.transform.SetPosition(2, 0.5, -3);
		quadObject.Render(camera);

		quadObject.transform.SetPosition(0, 0.5, 3);
		quadObject.Render(camera);

		quadObject.transform.SetPosition(1, 0.5, 3);
		quadObject.Render(camera);
	}
	
};

RegisterGameClass(Demo);
