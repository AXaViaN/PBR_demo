#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class NanosuitScene : public AX::Game {
public:
	ShaderProgram* phongShader;
	FreeCamera camera;
	SpotLight spotLight;

	Model<PhongMaterial> nanosuitModel;
	GameObject* nanosuitList[20];

	void Start()
	{
		Renderer::SetFullScreen(true);
		Renderer::SetWindowTitle("Nanosuit Demo Scene");
		phongShader = PhongMaterial().shader;
		ToneShader::SetHDRexposure(1);

		// Camera
		camera.transform.SetPosition(-1.9, 3.5, -2.5);
		camera.transform.SetRotation(7.5, 25.5, 0);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		spotLight.SetPosition(-3, 6, 0);
		spotLight.SetDirection(0.5, -0.5, -1.0);

		// Models
		nanosuitModel.Load("Test/Data/Nanosuit/nanosuit.obj");
		for( auto& nanosuit : nanosuitList )
			nanosuit = nanosuitModel.InstantiateGameObject();

		for( SIZE y=0 ; y<5 ; y++ )
		{
			for( SIZE x=0 ; x<4 ; x++ )
			{
				nanosuitList[y*4 + x]->transform.SetPosition(-2.25+(x*2.25), 0, -4.0-(y*2));
				nanosuitList[y*4 + x]->transform.Scale(0.25);
			}
		}
	}
	void Dispose()
	{
		nanosuitModel.Dispose();
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

		camera.Update();
	}

	void Draw()
	{
		Renderer::Clear(0, 0, 0);
		Renderer::PrepareScene(camera, spotLight);

		for( auto& nanosuit : nanosuitList )
			nanosuit->Render();
	}

};

RegisterGameClass(NanosuitScene);
