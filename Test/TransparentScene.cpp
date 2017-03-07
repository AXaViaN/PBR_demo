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
	GameObject testScene;

	Model<PhongMaterial> windowModel;
	GameObject windowList[4];

	void Start()
	{
		Input::ActivateMouseMotion(false);
		phongShader = PhongMaterial().shader;

		// Camera
		camera.transform.SetPosition(-1, 2, 2);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		sunLight.SetDirection(0.4, -1, -0.6);

		// Models
		testSceneModel.Load("Test/Data/TransparentScene/transparent_scene.obj");
		testScene = testSceneModel.InstantiateGameObject();
		testScene.transform.Rotate(0, -90, 0);

		for( SIZE i=0 ; i<testScene.GetChildCount() ; i++ )
		{
			if(testScene.GetChild(i)->name.find("Grass") != std::string::npos)
			{
				testScene.GetChild(i)->useTransparency = true;
			}
		}

		windowModel.Load("Test/Data/TransparentScene/window.obj");
		for( SIZE i=0 ; i<4 ; i++ )
			windowList[i] = windowModel.InstantiateGameObject();
		windowList[0].GetChild(0)->useTransparency = true;

		windowList[0].transform.SetPosition(-1, 1, -1.75);
		windowList[1].transform.SetPosition(-3, 1, -1.75);
		windowList[2].transform.SetPosition(-2, 1, -0.75);
		windowList[2].transform.SetRotation(0, 90, 0);
		windowList[3].transform.SetPosition(-2, 1, -2.75);
		windowList[3].transform.SetRotation(0, 90, 0);
	}
	void Dispose()
	{
		testSceneModel.Dispose();
		windowModel.Dispose();
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

		testScene.Render();
		
		std::map<float, GameObject*> winMap;
		for( SIZE i=0 ; i<4 ; i++ )
		{
			winMap[glm::length(camera.transform.position - windowList[i].transform.position)] = &windowList[i];
		}
		for( std::map<float, GameObject*>::reverse_iterator it=winMap.rbegin() ; it!=winMap.rend() ; it++ )
			it->second->Render();
	}

};

RegisterGameClass(TransparentScene);
