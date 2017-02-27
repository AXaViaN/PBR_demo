#include "AXengine/Game.h"

#include <glm/gtx/rotate_vector.hpp>

using namespace AX::Entity;
using namespace AX::Gfx;
using namespace AX::Model;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	FreeCamera camera;
	DirectionalLight directionalLight;
	PointLight pointLight;
	SpotLight spotLight;

	// Draw light to scene for debug
	Mesh lightModel;
	Material lightMaterial;
	GameObject pointLightObject;
	GameObject directionalLightObject;

	Mesh cubeModel;
	Texture cubeDiffuseTexture;
	Texture cubeSpecularTexture;
	Texture cubeEmissionTexture;
	PhongMaterial cubeMaterial;
	GameObject cubeList[5];

	Mesh surfaceModel;
	Texture surfaceDiffuseTexture;
	PhongMaterial surfaceMaterial;
	GameObject surface;

	void Start()
	{
		// Cube
		F32 vertexList[] = {
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f
		};
		F32 normalList[] = {
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,

			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,

			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,

			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,

			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,

			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f
		};
		F32 uvCoordList[] = {
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0
		};
		U32 indexList[] = {
			 0,  1,  3,
			 3,  1,  2,
			 4,  5,  7,
			 7,  5,  6,
			 8,  9, 11,
			11,  9, 10,
			12, 13, 15,
			15, 13, 14,
			16, 17, 19,
			19, 17, 18,
			20, 21, 23,
			23, 21, 22
		};
		cubeModel = Loader::LoadMesh(vertexList, sizeof(vertexList), normalList, sizeof(normalList), uvCoordList, sizeof(uvCoordList), indexList, sizeof(indexList));
		cubeDiffuseTexture = Loader::LoadTexture("Test/Data/crate_diff.png", true);
		cubeSpecularTexture = Loader::LoadTexture("Test/Data/crate_spec.png", true);
		cubeEmissionTexture = Loader::LoadTexture("Test/Data/crate_emis.png", true);
		cubeMaterial.diffuseMap.texture = &cubeDiffuseTexture;
		cubeMaterial.specularMap.texture = &cubeSpecularTexture;
		cubeMaterial.emissionMap.texture = &cubeEmissionTexture;
		
		for( GameObject& cube : cubeList )
			cube = GameObject(cubeModel, cubeMaterial);
		cubeList[0].transform.SetPosition(0, 0.5, -3);
		cubeList[1].transform.SetPosition(0, 1.5, -3);
		cubeList[2].transform.SetPosition(2, 0.5, -3);

		cubeList[3].transform.SetPosition(1, 0.5, 3);
		cubeList[3].transform.SetRotation(25, 0, 0);
		cubeList[3].transform.SetScale(2, 0.75, 1);

		cubeList[4].transform.SetPosition(-2, 0.5, 3);
		cubeList[4].transform.SetRotation(0, 45, 0);
		
		// Surface
		F32 surfaceVertex[] = {
			-1,  0, -1,
			-1,  0,  1,
			 1,  0, -1,
			 1,  0,  1
		};
		F32 surfaceNormal[] = {
			 0,  1,  0,
			 0,  1,  0,
			 0,  1,  0,
			 0,  1,  0
		};
		F32 surfaceUV[] = {
			0, 0,
			0, 1,
			1, 0,
			1, 1
		};
		U32 surfaceIndex[] = {
			0, 1, 2,
			2, 1, 3
		};
		surfaceModel = Loader::LoadMesh(surfaceVertex, sizeof(surfaceVertex), surfaceNormal, sizeof(surfaceNormal), surfaceUV, sizeof(surfaceUV), surfaceIndex, sizeof(surfaceIndex));
		surfaceDiffuseTexture = Loader::LoadTexture("Test/Data/dirt_diff.png", true);
		surfaceMaterial.diffuseMap.texture = &surfaceDiffuseTexture;
		
		surface = GameObject(surfaceModel, surfaceMaterial);
		surface.transform.Scale(5);
		
		// Camera
		camera.transform.Translate(0, 1, 0);
		camera.SetMovementSpeed(1.5);
		camera.SetRotationSpeed(1.5);

		// Lights
		directionalLight.transform.SetRotation(-75, -45, 30);
		directionalLight.SetDiffuseIntensity(0.2, 0.2, 0.2);
		directionalLight.SetSpecularIntensity(0.5, 0.5, 0.5);
		directionalLight.SetAmbientIntensity(0.05, 0.05, 0.05);

		spotLight.transform.SetPosition(2, 5, -3);
		spotLight.transform.SetRotation(-90, 0, 0);
		spotLight.SetDiffuseIntensity(0.4, 0.4, 0.1);
		spotLight.SetSpecularIntensity(0.6, 0.6, 0.15);
		spotLight.SetAmbientIntensity(0.1, 0.1, 0.05);

		pointLight.transform.SetPosition(0, 2, 0);
		pointLight.SetDiffuseIntensity(0.3, 0.1, 0.5);
		pointLight.SetSpecularIntensity(0.5, 0.1, 0.8);
		pointLight.SetAmbientIntensity(0.05, 0.01, 0.1);
		pointLight.linear = 0.5;
		pointLight.quadric = 0.4;
		// Light model (debug)
		lightModel  = Loader::LoadMesh(vertexList, sizeof(vertexList), indexList, sizeof(indexList));
		lightMaterial.diffuseMap.value = pointLight.diffuseIntensity;
		pointLightObject = GameObject(lightModel, lightMaterial);
		pointLightObject.transform.Scale(0.125);
		directionalLightObject = GameObject(lightModel, lightMaterial);
		glm::vec3 directionalLightDirection = glm::vec3(0, 0, -1);
		directionalLightDirection = glm::rotate(directionalLightDirection, glm::radians(directionalLight.transform.rotation.x), glm::vec3(1, 0, 0));
		directionalLightDirection = glm::rotate(directionalLightDirection, glm::radians(directionalLight.transform.rotation.y), glm::vec3(0, 1, 0));
		directionalLightDirection = glm::rotate(directionalLightDirection, glm::radians(directionalLight.transform.rotation.z), glm::vec3(0, 0, 1));
		directionalLightObject.transform.Translate(glm::vec3(-50, -50, -50) * directionalLightDirection);

		Input::ActivateMouseMotion(false);
	}
	void Dispose()
	{
		lightModel.Dispose();

		cubeModel.Dispose();
		cubeDiffuseTexture.Dispose();
		cubeSpecularTexture.Dispose();
		cubeEmissionTexture.Dispose();

		surfaceModel.Dispose();
		surfaceDiffuseTexture.Dispose();
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
			cubeMaterial.shader->SetDebugDrawMode(isDebugMode=!isDebugMode);
		}

		if(Input::GetMouseButtonDown(Input::MouseButton::LEFT))
		{
			Input::ActivateMouseMotion(true);
		}
		if(Input::GetMouseButtonDown(Input::MouseButton::RIGHT))
		{
			Input::ActivateMouseMotion(false);
		}

		glm::vec3 lightTranslation;
		if(Input::GetKey(SDL_SCANCODE_KP_8))
			lightTranslation.z -= 1;
		if(Input::GetKey(SDL_SCANCODE_KP_2))
			lightTranslation.z += 1;
		if(Input::GetKey(SDL_SCANCODE_KP_6))
			lightTranslation.x += 1;
		if(Input::GetKey(SDL_SCANCODE_KP_4))
			lightTranslation.x -= 1;
		if(Input::GetKey(SDL_SCANCODE_KP_PLUS))
			lightTranslation.y += 1;
		if(Input::GetKey(SDL_SCANCODE_KP_ENTER))
			lightTranslation.y -= 1;
		pointLight.transform.Translate(lightTranslation * Time::GetDeltaTime());
		pointLightObject.transform.position = pointLight.transform.position;
		
		camera.Update();
	}
	
	void Draw()
	{
		Renderer::Clear(0.1, 0.1, 0.1);

		const Light* lightList[] = {&directionalLight, &pointLight, &spotLight};

		lightMaterial.diffuseMap.value = directionalLight.diffuseIntensity * 10.0f;
		directionalLightObject.Render(camera);

		lightMaterial.diffuseMap.value = pointLight.diffuseIntensity;
		pointLightObject.Render(camera);

		surface.Render(camera, lightList, sizeof(lightList));

		for( GameObject& cube : cubeList )
			cube.Render(camera, lightList, sizeof(lightList));
	}
	
};

RegisterGameClass(Demo);
