#include "AXengine/Game.h"

using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Asset;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	FreeCamera camera;
	DirectionalLight directionalLight;

	Mesh cubeModel;
	Texture cubeDiffuseTexture;
	Texture cubeSpecularTexture;
	PhongMaterial cubeMaterial;
	GameObject cubeList[5];

	Mesh surfaceModel;
	Texture surfaceDiffuseTexture;
	PhongMaterial surfaceMaterial;
	GameObject surface;

	void Start()
	{
		// Cube
		F32 vertexList[] = {
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f
		};
		F32 normalList[] = {
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,

			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,

			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,

			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,

			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,

			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f
		};
		F32 uvCoordList[] = {
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0
		};
		U32 indexList[] = {
			 0,  1,  3,
			 3,  1,  2,
			 4,  5,  7,
			 7,  5,  6,
			 8,  9, 11,
			11,  9, 10,
			12, 13, 15,
			15, 13, 14,
			16, 17, 19,
			19, 17, 18,
			20, 21, 23,
			23, 21, 22
		};
		cubeModel = Loader::LoadMesh(vertexList, sizeof(vertexList), normalList, sizeof(normalList), uvCoordList, sizeof(uvCoordList), indexList, sizeof(indexList));
		cubeDiffuseTexture = Loader::LoadTexture("Test/Data/crate_diff.png", true);
		cubeSpecularTexture = Loader::LoadTexture("Test/Data/crate_spec.png", true);
		cubeMaterial.diffuseMap.texture = &cubeDiffuseTexture;
		cubeMaterial.specularMap.texture = &cubeSpecularTexture;
		
		for( GameObject& cube : cubeList )
			cube = GameObject(cubeModel, cubeMaterial);
		cubeList[0].transform.SetPosition(0, 0.5, -3);
		cubeList[1].transform.SetPosition(0, 1.5, -3);
		cubeList[2].transform.SetPosition(2, 0.5, -3);

		cubeList[3].transform.SetPosition(1, 0.5, 3);
		cubeList[3].transform.SetRotation(25, 0, 0);
		cubeList[3].transform.SetScale(2, 0.75, 1);

		cubeList[4].transform.SetPosition(-2, 0.5, 3);
		cubeList[4].transform.SetRotation(0, 45, 0);
		
		// Surface
		F32 surfaceVertex[] = {
			-1,  0, -1,
			-1,  0,  1,
			 1,  0, -1,
			 1,  0,  1
		};
		F32 surfaceNormal[] = {
			 0,  1,  0,
			 0,  1,  0,
			 0,  1,  0,
			 0,  1,  0
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
		surfaceModel = Loader::LoadMesh(surfaceVertex, sizeof(surfaceVertex), surfaceNormal, sizeof(surfaceNormal), surfaceUV, sizeof(surfaceUV), surfaceIndex, sizeof(surfaceIndex));
		surfaceDiffuseTexture = Loader::LoadTexture("Test/Data/dirt_diff.png", true);
		surfaceMaterial.diffuseMap.texture = &surfaceDiffuseTexture;
		surfaceMaterial.specularMap.value = glm::vec3(0.05, 0.05, 0.05);
		
		surface = GameObject(surfaceModel, surfaceMaterial);
		surface.transform.Scale(5);
		
		// Camera
		camera.transform.SetPosition(0, 1, 0);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		directionalLight.SetDirection(0.3, -1, -0.7);

		Input::ActivateMouseMotion(false);
	}
	void Dispose()
	{
		cubeModel.Dispose();
		cubeDiffuseTexture.Dispose();
		cubeSpecularTexture.Dispose();

		surfaceModel.Dispose();
		surfaceDiffuseTexture.Dispose();
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
		Renderer::PrepareScene(camera, directionalLight);

		surface.Render();

		for( GameObject& cube : cubeList )
			cube.Render();
	}
	
};

RegisterGameClass(Demo);
