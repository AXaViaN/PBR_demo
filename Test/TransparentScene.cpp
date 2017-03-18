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

	Text engineText;
	Text cameraPositionText;
	Text cameraRotationText;

	Cubemap skybox;

	void Start()
	{
		TextRenderer::Init("Test/Data/arial.ttf");
		Input::ActivateMouseMotion(false);
		phongShader = PhongMaterial().shader;

		// Camera
		camera.transform.SetPosition(0, 1, 2);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		sunLight.SetDirection(0.4, -0.8, -0.75);

		// Models
		testSceneModel.Load("Test/Data/TransparentScene/transparent_scene.obj");
		testScene = testSceneModel.InstantiateGameObject();
		testScene->transform.Rotate(0, -90, 0);
		for( SIZE i=0 ; i<testScene->GetChildCount() ; i++ )
			testScene->GetChild(i)->material->environmentMap = &skybox;
		
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

		// Text
		engineText.text = "AXengine Gfx Demo";
		engineText.size = 28;
		engineText.color = glm::vec3(0.5, 0.3, 1.0);
		engineText.position = glm::vec2(1000, 10);
		engineText.backgroundColor = glm::vec4(0, 0, 0, 0.7);
		engineText.backgroundBorder = glm::vec2(5, 5);

		cameraPositionText.size = 20;
		cameraPositionText.color = glm::vec3(1, 1, 1);
		cameraPositionText.position = glm::vec2(10, 690);
		cameraPositionText.backgroundColor = glm::vec4(0, 0, 0, 0.5);
		cameraPositionText.backgroundBorder = glm::vec2(5, 5);

		cameraRotationText = cameraPositionText;
		cameraRotationText.position.y -= 35;

		// Skybox
		skybox.Load("Test/Data/Skybox/Above The Sea/right.png",
					"Test/Data/Skybox/Above The Sea/left.png",
					"Test/Data/Skybox/Above The Sea/top.png",
					"Test/Data/Skybox/Above The Sea/bottom.png",
					"Test/Data/Skybox/Above The Sea/back.png",
					"Test/Data/Skybox/Above The Sea/front.png");
	}
	void Dispose()
	{
		skybox.Dispose();

		testSceneModel.Dispose();
		windowModel.Dispose();
		grassModel.Dispose();

		TextRenderer::Terminate();
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

		cameraPositionText.text = "CamPos:";
		cameraPositionText.text += " x=";
		cameraPositionText.text += std::to_string(camera.transform.position.x);
		cameraPositionText.text += " y=";
		cameraPositionText.text += std::to_string(camera.transform.position.y);
		cameraPositionText.text += " z=";
		cameraPositionText.text += std::to_string(camera.transform.position.z);

		cameraRotationText.text = "CamRot:";
		cameraRotationText.text += " x=";
		cameraRotationText.text += std::to_string(camera.transform.rotation.x);
		cameraRotationText.text += " y=";
		cameraRotationText.text += std::to_string(camera.transform.rotation.y);
		cameraRotationText.text += " z=";
		cameraRotationText.text += std::to_string(camera.transform.rotation.z);
	}

	void Draw()
	{
		Renderer::PrepareScene(camera, sunLight);
		SkyboxRenderer::Render(skybox);

		testScene->Render();

		for( auto& window : windowList )
			window->Render();
		for( auto& grass : grassList )
			grass->Render();

		// Render text
		engineText.Render();
		cameraPositionText.Render();
		cameraRotationText.Render();
	}

};

RegisterGameClass(TransparentScene);
