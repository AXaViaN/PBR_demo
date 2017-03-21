#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class WarehouseScene : public AX::Game {
public:
	ShaderProgram* phongShader;
	
	Text cameraPositionText;
	Text cameraRotationText;

	FreeCamera camera;
	PointLight lightList[4];

	Model<PhongMaterial> warehouseModel;
	GameObject* warehouse;

	void Start()
	{
		// Setup
		TextRenderer::Init("Test/Data/arial.ttf");
		Input::ActivateMouseMotion(false);
		phongShader = PhongMaterial().shader;

		// Set text
		cameraPositionText.size = 20;
		cameraPositionText.color = glm::vec3(1, 1, 1);
		cameraPositionText.position = glm::vec2(10, 690);
		cameraPositionText.backgroundColor = glm::vec4(0, 0, 0, 0.5);
		cameraPositionText.backgroundBorder = glm::vec2(5, 5);

		cameraRotationText = cameraPositionText;
		cameraRotationText.position.y -= 35;

		// Set camera
		camera.transform.SetPosition(1.7, 0.75, 6.65);
		camera.transform.SetRotation(2.15, -29.0, 0);

		// Set game object
		warehouseModel.Load("Test/Data/Warehouse/warehouse.obj");
		warehouse = warehouseModel.InstantiateGameObject();

		// Set lights
		lightList[0].SetPosition(0.0, 1.0, -6);
		lightList[0].SetDiffuseIntensity(50.0, 50.0, 0.0);
		lightList[0].SetSpecularIntensity(1.0, 1.0, 0.0);
		lightList[0].SetAmbientIntensity(0.1, 0.1, 0.0);

		lightList[1].SetPosition(2.0, 0.5, -1.0);
		lightList[1].SetDiffuseIntensity(1.0, 0.0, 0.0);
		lightList[1].SetSpecularIntensity(1.0, 0.0, 0.0);
		lightList[1].SetAmbientIntensity(0.1, 0.0, 0.0);

		lightList[2].SetPosition(3.0, 1.0, 6.0);
		lightList[2].SetDiffuseIntensity(0.0, 1.0, 0.0);
		lightList[2].SetSpecularIntensity(0.0, 1.0, 0.0);
		lightList[2].SetAmbientIntensity(0.0, 0.1, 0.0);

		lightList[3].SetPosition(-3.0, 0.5, 5.0);
		lightList[3].SetDiffuseIntensity(0.0, 0.0, 1.0);
		lightList[3].SetSpecularIntensity(0.0, 0.0, 1.0);
		lightList[3].SetAmbientIntensity(0.0, 0.0, 0.1);
	}
	void Dispose()
	{
		warehouseModel.Dispose();

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
			if(isDebugMode)
				ToneShader::SetGamma(1);
			else
				ToneShader::SetGamma(2);
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
		const Light* sceneLights[] = {
			&lightList[0], &lightList[1], &lightList[2], &lightList[3]
		};
		Renderer::PrepareScene(camera, sceneLights, sizeof(sceneLights));

		warehouse->Render();

		cameraPositionText.Render();
		cameraRotationText.Render();
	}

};

RegisterGameClass(WarehouseScene);
