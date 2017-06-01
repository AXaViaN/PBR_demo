#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class PBRscene : public AX::Game {
public:
	FreeCamera camera;
	Cubemap skyboxList[6];
	EnvironmentProbe probeList[6];

	Model<PBRMaterial> sphereModel;
	GameObject* sphere;

	Cubemap* currentSkybox;
	EnvironmentProbe* currentProbe;

	Text infoText;

	void Start()
	{
		// Setup
		Input::ActivateMouseMotion(false);
		TextRenderer::Init("Test/Data/arial.ttf");

		// Set Camera
		camera.transform.SetPosition(0, 0, 2);

		// Set game object
		sphereModel.Load("Test/Data/Sphere/sphere.obj", true);
		sphere = sphereModel.InstantiateGameObject();
		
		// Skybox
		skyboxList[0].Load("Test/Data/Skybox/GCanyon.hdr");
		skyboxList[1].Load("Test/Data/Skybox/Tokyo_BigSight.hdr");
		skyboxList[2].Load("Test/Data/Skybox/Topanga_Forest.hdr");
		skyboxList[3].Load("Test/Data/Skybox/Tropical_Beach.hdr");
		skyboxList[4].Load("Test/Data/Skybox/TropicalRuins.hdr");
		skyboxList[5].Load("Test/Data/Skybox/WinterForest.hdr");

		// Environment
		EnvironmentProbe::InitCaptureShader();

		for( SIZE i=0 ; i<6 ; i++ )
		{
			probeList[i].Init(512);
			currentSkybox = &skyboxList[i];
			probeList[i].Capture([](void* sceneInstance){
				SkyboxRenderer::Render(*(reinterpret_cast<PBRscene*>(sceneInstance)->currentSkybox));
			}, this);
		}

		EnvironmentProbe::TerminateCaptureShader();

		currentSkybox = &skyboxList[0];
		currentProbe = &probeList[0];

		// Text
		infoText.text = "[1-6] Change environment    "
						"[SPACE] Rotate camera around sphere    "
						"[LMB] Enter FreeCam    "
						"[RMB] Exit FreeCam    "
						"[F] Fullscreen    "
						"[ESC] Exit";
		infoText.size = 18;
		infoText.color = glm::vec3(1.0, 1.0, 1.0);
		infoText.position = glm::vec2(50, 10);
		infoText.backgroundColor = glm::vec4(0, 0, 0, 0.7);
		infoText.backgroundBorder = glm::vec2(5, 5);
	}
	void Dispose()
	{
		for( SIZE i=0 ; i<6 ; i++ )
		{
			skyboxList[i].Dispose();
			probeList[i].Dispose();
		}

		sphereModel.Dispose();
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

		for( SIZE i=0 ; i<6 ; i++ )
		{
			if(Input::GetKeyDown(SDL_SCANCODE_1 + i))
			{
				currentSkybox = &skyboxList[i];
				currentProbe = &probeList[i];
				break;
			}
		}

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
		Renderer::PrepareScene(camera);
		Renderer::SetSceneEnvironment(currentProbe);
		
		SkyboxRenderer::Render(*currentSkybox);

		sphere->Render();

		infoText.Render();
	}

};

RegisterGameClass(PBRscene);
