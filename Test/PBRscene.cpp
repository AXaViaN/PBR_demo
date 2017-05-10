#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class PBRscene : public AX::Game {
public:
	FreeCamera camera;
	DirectionalLight light;
	Cubemap daySkybox;
	Cubemap sunsetSkybox;
	Cubemap nightSkybox;

	Model<PBRMaterial> sphereModel;
	GameObject* sphere;

	EnvironmentProbe dayProbe;
	EnvironmentProbe sunsetProbe;
	EnvironmentProbe nightProbe;

	Cubemap* currentSkybox;
	Cubemap* currentEnvironment;

	void Start()
	{
		// Setup
		Input::ActivateMouseMotion(false);

		// Set Camera
		camera.transform.SetPosition(0, 0, 2);

		// Set game object
		sphereModel.Load("Test/Data/Sphere/sphere.obj", true);
		sphere = sphereModel.InstantiateGameObject();
		
		// DEBUG
		Renderer::Clear(0.05, 0.05, 0.05);
		ToneShader::SetHDRexposure(2);
		
		// Set light
		light.SetDirection(-1.0, 0.2, -0.3);

		// Skybox
		daySkybox.Load("Test/Data/Skybox/Above The Sea/right.png",
						"Test/Data/Skybox/Above The Sea/left.png",
						"Test/Data/Skybox/Above The Sea/top.png",
						"Test/Data/Skybox/Above The Sea/bottom.png",
						"Test/Data/Skybox/Above The Sea/back.png",
						"Test/Data/Skybox/Above The Sea/front.png");
		sunsetSkybox.Load("Test/Data/Skybox/Sunset/right.png",
						"Test/Data/Skybox/Sunset/left.png",
						"Test/Data/Skybox/Sunset/top.png",
						"Test/Data/Skybox/Sunset/bottom.png",
						"Test/Data/Skybox/Sunset/back.png",
						"Test/Data/Skybox/Sunset/front.png");
		nightSkybox.Load("Test/Data/Skybox/Grimm Night/right.png",
						"Test/Data/Skybox/Grimm Night/left.png",
						"Test/Data/Skybox/Grimm Night/top.png",
						"Test/Data/Skybox/Grimm Night/bottom.png",
						"Test/Data/Skybox/Grimm Night/back.png",
						"Test/Data/Skybox/Grimm Night/front.png");

		currentSkybox = &daySkybox;

		// Environment
		EnvironmentProbe::InitConvolutionShader();
		dayProbe.Init(512);
		dayProbe.Capture([](void* sceneInstance){
			SkyboxRenderer::Render(reinterpret_cast<PBRscene*>(sceneInstance)->daySkybox);
		}, this);

		sunsetProbe.Init(512);
		sunsetProbe.Capture([](void* sceneInstance){
			SkyboxRenderer::Render(reinterpret_cast<PBRscene*>(sceneInstance)->sunsetSkybox);
		}, this);

		nightProbe.Init(512);
		nightProbe.Capture([](void* sceneInstance){
			SkyboxRenderer::Render(reinterpret_cast<PBRscene*>(sceneInstance)->nightSkybox);
		}, this);

		EnvironmentProbe::TerminateConvolutionShader();

		currentEnvironment = &dayProbe.GetEnvironmentMap();
	}
	void Dispose()
	{
		sphereModel.Dispose();

		daySkybox.Dispose();
		sunsetSkybox.Dispose();
		nightSkybox.Dispose();

		dayProbe.Dispose();
		sunsetProbe.Dispose();
		nightProbe.Dispose();
	}

	void Update()
	{
		if(Input::GetKeyUp(SDL_SCANCODE_ESCAPE))
		{
			Debug::LogInfo("Escaped!");
			Exit();
		}
		if(Input::GetKeyDown(SDL_SCANCODE_F))
		{
			static bool isFullScreen = false;
			Renderer::SetFullScreen(isFullScreen=!isFullScreen);
		}

		if(Input::GetMouseButtonDown(Input::MouseButton::LEFT))
			Input::ActivateMouseMotion(true);
		if(Input::GetMouseButtonDown(Input::MouseButton::RIGHT))
			Input::ActivateMouseMotion(false);

		if(Input::GetKeyDown(SDL_SCANCODE_1))
		{
			currentSkybox = &daySkybox;
			currentEnvironment = &dayProbe.GetEnvironmentMap();
		}
		if(Input::GetKeyDown(SDL_SCANCODE_2))
		{
			currentSkybox = &sunsetSkybox;
			currentEnvironment = &sunsetProbe.GetEnvironmentMap();
		}
		if(Input::GetKeyDown(SDL_SCANCODE_3))
		{
			currentSkybox = &nightSkybox;
			currentEnvironment = &nightProbe.GetEnvironmentMap();
		}
		
		static bool isUsingEnvironment = true;
		if(Input::GetKeyDown(SDL_SCANCODE_TAB))
			isUsingEnvironment = !isUsingEnvironment;

		if(isUsingEnvironment)
			sphere->GetChild(0)->material->environmentMap = currentEnvironment;
		else
			sphere->GetChild(0)->material->environmentMap = nullptr;

		static bool isAutoRotating = false;
		if(Input::GetKeyDown(SDL_SCANCODE_SPACE))
			isAutoRotating = !isAutoRotating;

		if(isAutoRotating)
		{
			static F32 angle = 90;
			angle += 30 * Time::GetDeltaTime();
			glm::vec3 position = glm::vec3(glm::cos(glm::radians(angle)), 0, glm::sin(glm::radians(angle))) * 2.5f;
			glm::vec3 rotation = glm::vec3(0, angle-90, 0);

			camera.transform.Translate((position - camera.transform.position)*Time::GetDeltaTime());
			camera.transform.Rotate((rotation - camera.transform.rotation)*Time::GetDeltaTime());
		}
		else
		{
			camera.Update();
		}
	}
	void Draw()
	{
		Renderer::PrepareScene(camera, light);
		SkyboxRenderer::Render(*currentSkybox);

		sphere->Render();
	}

};

RegisterGameClass(PBRscene);
