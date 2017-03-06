#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	ShaderProgram* phongShader;
	FreeCamera camera;
	PointLight light[2];
	PointLight roomLight;
	bool isNightTime = false;

	Model<PhongMaterial> bedroomModel;
	GameObject bedroom;

	Model<PhongMaterial> monkeyModel;
	GameObject monkey;
	GameObject monkey2;

	void Start()
	{
		Input::ActivateMouseMotion(false);
		phongShader = PhongMaterial().shader;

		// Camera
		camera.transform.SetPosition(0, 5, 5);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		roomLight.SetPosition(0, 8, 0);
		roomLight.linear = 0.005;
		roomLight.quadric = 0.001;

		light[0].SetPosition(5, 4, -6.75);
		light[1].SetPosition(-5, 4, -6.75);
		for( auto& light_ : light )
		{
			light_.SetDiffuseIntensity(0.9, 0.9, 0.2);
			light_.SetSpecularIntensity(1.0, 1.0, 0.3);
			light_.SetAmbientIntensity(0.2, 0.2, 0.01);
			light_.quadric = 0.01;
		}
		
		// Models
		bedroomModel.Load("Test/Data/Bedroom/Bedroom.obj");
		bedroom = bedroomModel.InstantiateGameObject();
		bedroom.transform.SetRotation(0, 180, 0);
		
		monkeyModel.Load("Test/Data/monkey.obj");
		monkey = monkeyModel.InstantiateGameObject();
		monkey.transform.SetPosition(-1.5, 4, 0);
		monkey.transform.SetRotation(-30, 0, 0);
		monkey.layer = 2;

		monkey2 = monkeyModel.InstantiateGameObject();
		monkey2.transform.SetPosition(1.5, 4, 0);
		monkey2.transform.SetRotation(-30, 0, 0);
		monkey2.layer = 3;
	}
	void Dispose()
	{
		bedroomModel.Dispose();
		monkeyModel.Dispose();
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

		if(Input::GetKeyDown(SDL_SCANCODE_SPACE))
			isNightTime = !isNightTime;

		camera.Update();
	}
	
	void Draw()
	{
		Renderer::Clear(0.05, 0.05, 0.05);
		if(isNightTime)
		{
			const Light* lightList[] = {&light[0], &light[1]};
			Renderer::PrepareScene(camera, lightList, sizeof(lightList));
		}
		else
		{
			Renderer::PrepareScene(camera, roomLight);
		}
		
		bedroom.Render();

		monkey.Render();
		monkey2.Render();

		monkey.RenderOutline(glm::vec3(0.8, 0.8, 0), 0.1);
		monkey2.RenderOutline(glm::vec3(0.8, 0, 0.8), 0.25);
	}
	
};

RegisterGameClass(Demo);
