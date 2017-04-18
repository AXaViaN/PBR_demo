#include "AXengine/Game.h"

using namespace AX::Asset;
using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Shader;
using namespace AX::Tool;

// DEBUG
#include "AXengine/Asset/PBRMaterial.h"

class PBRscene : public AX::Game {
public:
	FreeCamera camera;
	DirectionalLight light;
	Cubemap skybox;

	Model<PhongMaterial> sphereModel;
	GameObject* sphere;
	
	GameObject* sphereList[54];
	PBRMaterial pbrMaterialList[54];
	PointLight pointLightList[4];

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
		pointLightList[0].SetPosition(7, 7, 10);
		pointLightList[1].SetPosition(-7, 7, 10);
		pointLightList[2].SetPosition(7, -7, 10);
		pointLightList[3].SetPosition(-7, -7, 10);
		for( auto& pointLight : pointLightList )
			pointLight.diffuseIntensity = glm::vec3(5,5,5);
		camera.transform.SetPosition(0, 0, 6);

		/*auto phongMaterial = sphere->GetChild(0)->material->Get<PhongMaterial>();
		pbrMaterialList[0].diffuseMap.texture = phongMaterial->diffuseMap.texture;
		pbrMaterialList[0].normalMap.texture = phongMaterial->normalMap.texture;
		pbrMaterialList[0].metallicMap.texture = phongMaterial->specularMap.texture;
		pbrMaterialList[0].roughnessMap.texture = phongMaterial->reflectionMap.texture;
		sphere->GetChild(0)->material = &pbrMaterialList[0];
		return;*/
		
		for( int y=0 ; y<6 ; y++ )
		{
			for( int x=0 ; x<9 ; x++ )
			{
				int pos = x + y*9;
				sphereList[pos] = sphereModel.InstantiateGameObject();
				sphereList[pos]->GetChild(0)->material = &pbrMaterialList[pos];

				sphereList[pos]->transform.SetPosition(x*1.1-4, y*1.2-3, 0);
				sphereList[pos]->transform.SetScale(0.5);

				pbrMaterialList[pos].diffuseMap.value = glm::vec4(1, 0, 0, 1);
				pbrMaterialList[pos].metallicMap.value = 0.2 + x*0.05;
				pbrMaterialList[pos].roughnessMap.value = 0.9 - y*0.15;
			}
		}
		
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
		// DEBUG
		/*Renderer::PrepareScene(camera, light);
		SkyboxRenderer::Render(skybox);

		sphere->Render();*/

		/*Renderer::PrepareScene(camera, pointLight);
		sphere->Render();*/

		const Light* lightList[] = {
			&pointLightList[0], &pointLightList[1], &pointLightList[2], &pointLightList[3]
		};
		Renderer::PrepareScene(camera, lightList, sizeof(lightList));

		for( auto& sphere_ : sphereList )
			sphere_->Render();
	}

};

RegisterGameClass(PBRscene);
