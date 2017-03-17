#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

class PostProcessScene : public AX::Game {
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

	FrameBuffer sceneFBO;
	FrameBuffer sceneRearFBO;
	Quad sceneFBOquad;
	Quad sceneRearFBOquad;

	void Start()
	{
		sceneFBO.Init({1280, 720}, FrameBuffer::AttachmentType::COLOR_TEXTURE | FrameBuffer::AttachmentType::DEPTH_STENCIL_BUFFER);
		// Apply kernel shader to scene front quad
		F32 kernel[] = {
			-2, -1,  0,
			-1,  1,  1,
			0,  1,  2
		};
		KernelShader* kernelShader = KernelShader::SetKernel(kernel, 0.003);
		sceneFBOquad.material.shader = kernelShader;
		sceneFBOquad.material.diffuseMap.texture = &sceneFBO.GetColorTexture();

		sceneRearFBO.Init({1280, 720}, FrameBuffer::AttachmentType::COLOR_TEXTURE | FrameBuffer::AttachmentType::DEPTH_STENCIL_BUFFER);
		sceneRearFBOquad.material.shader = kernelShader;
		sceneRearFBOquad.material.diffuseMap.texture = &sceneRearFBO.GetColorTexture();
		sceneRearFBOquad.transform.SetPosition(0, 0.75, 0);
		sceneRearFBOquad.transform.SetScale(0.25, 0.2, 1);

		TextRenderer::Init("Test/Data/arial.ttf");
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
	}
	void Dispose()
	{
		testSceneModel.Dispose();
		windowModel.Dispose();
		grassModel.Dispose();

		TextRenderer::Terminate();

		sceneRearFBO.Terminate();
		sceneFBO.Terminate();
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
		// Render scene to sceneFBOquad
		sceneFBO.Use();
		RenderScene();
		// Create an empty frame
		Quad sceneRearFrame;
		sceneRearFrame.transform = sceneRearFBOquad.transform;
		sceneRearFrame.transform.Scale(1.05);
		sceneRearFrame.material.diffuseMap.value = glm::vec4(0, 0, 0, 1);
		sceneRearFrame.RenderImmediate();

		FrameBuffer::UseDefault();
		sceneFBOquad.Render();

		// Render rear mirror to sceneRearFBOquad
		sceneRearFBO.Use();
		camera.transform.Rotate(0, 180, 0);
		camera.transform.rotation.x *= -1;
		RenderScene();
		camera.transform.rotation.x *= -1;
		camera.transform.Rotate(0, -180, 0);

		FrameBuffer::UseDefault();
		sceneRearFBOquad.Render();

		// Render text
		engineText.Render();
		cameraPositionText.Render();
		cameraRotationText.Render();
	}

	void RenderScene()
	{
		Renderer::Clear(0.2, 0.4, 0.7);
		Renderer::PrepareScene(camera, sunLight);

		testScene->Render();

		for( auto& window : windowList )
			window->Render();
		for( auto& grass : grassList )
			grass->Render();

		Renderer::RenderBatch();
	}

};

RegisterGameClass(PostProcessScene);
