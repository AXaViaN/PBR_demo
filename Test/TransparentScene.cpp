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
	}
	void Dispose()
	{
		testSceneModel.Dispose();
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
	}

};

RegisterGameClass(TransparentScene);
