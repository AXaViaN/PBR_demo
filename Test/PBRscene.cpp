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
	Cubemap skybox;

	Model<PhongMaterial> sphereModel;
	GameObject* sphere;

	void Start()
	{
		// Setup
		Input::ActivateMouseMotion(false);

		// Set Camera
		camera.transform.SetPosition(0, 0, 2);

		// Set game object
		sphereModel.Load("Test/Data/Sphere/sphere.obj", true);
		sphere = sphereModel.InstantiateGameObject();
		
		// Set light
		light.SetDirection(-1.0, 0.2, -0.3);

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
		SkyboxRenderer::Render(skybox);

		sphere->Render();
	}

};

RegisterGameClass(PBRscene);
