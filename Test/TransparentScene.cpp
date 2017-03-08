#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class TransparentScene : public AX::Game {
public:
	ShaderProgram* phongShader;
	FreeCamera camera;
	
	DirectionalLight sunLight;

	Model<PhongMaterial> testSceneModel;
	GameObject* testScene;

	Model<PhongMaterial> windowModel;
	GameObject* windowList[4];

	Model<PhongMaterial> grassModel;
	GameObject* grassList[10];

	void Start()
	{
		Input::ActivateMouseMotion(false);
		phongShader = PhongMaterial().shader;

		// Camera
		camera.transform.SetPosition(0, 1, 2);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		sunLight.SetDirection(0.4, -1, -0.6);

		// Models
		testSceneModel.Load("Test/Data/TransparentScene/transparent_scene.obj");
		testScene = testSceneModel.InstantiateGameObject();
		testScene->transform.Rotate(0, -90, 0);

		windowModel.Load("Test/Data/TransparentScene/window.obj");
		for( SIZE i=0 ; i<4 ; i++ )
		{
			windowList[i] = windowModel.InstantiateGameObject();
			windowList[i]->GetChild(0)->useTransparency = true;
		}

		windowList[0]->transform.SetPosition(-1, 1, -1.75);
		windowList[1]->transform.SetPosition(-3, 1, -1.75);
		windowList[2]->transform.SetPosition(-2, 1, -0.75);
		windowList[2]->transform.SetRotation(0, 90, 0);
		windowList[3]->transform.SetPosition(-2, 1, -2.75);
		windowList[3]->transform.SetRotation(0, 90, 0);

		grassModel.Load("Test/Data/TransparentScene/grass.obj");
		for( SIZE i=0 ; i<10 ; i++ )
		{
			grassList[i] = grassModel.InstantiateGameObject();
			grassList[i]->GetChild(0)->useTransparency = true;
			grassList[i]->GetChild(1)->useTransparency = true;

			grassList[i]->transform.SetRotation(0, std::rand()%360, 0);
		}
		grassList[0]->transform.SetPosition(0, 0, -2);
		grassList[1]->transform.SetPosition(2, 0, -3.5);
		grassList[2]->transform.SetPosition(-3, 0, -4);
		grassList[3]->transform.SetPosition(3, 0, 2);
		grassList[4]->transform.SetPosition(-4, 0, 3);
		grassList[5]->transform.SetPosition(-1.25, 0, 0.5);
		grassList[6]->transform.SetPosition(1, 0, 1);
		grassList[7]->transform.SetPosition(-1.5, 0, 2);
		grassList[8]->transform.SetPosition(1, 0, 4);
		grassList[9]->transform.SetPosition(1, 0, -1.5);
	}
	void Dispose()
	{
		testSceneModel.Dispose();
		windowModel.Dispose();
		grassModel.Dispose();
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
			phongShader->SetDebugDrawMode(isDebugMode=!isDebugMode);
		}
		if(Input::GetKeyDown(SDL_SCANCODE_F))
		{
			static bool isFullScreen = false;
			Renderer::SetFullScreen(isFullScreen=!isFullScreen);
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
		Renderer::Clear(0.2, 0.4, 0.7);
		Renderer::PrepareScene(camera, sunLight);

		testScene->Render();

		for( auto& window : windowList )
			window->Render();
		for( auto& grass : grassList )
			grass->Render();
	}

};

RegisterGameClass(TransparentScene);
