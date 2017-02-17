#include "AXengine/Game.h"

using namespace AX::Entity;
using namespace AX::Model;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	Camera camera;
	GameObject quadObject;
	Mesh quadModel;
	Texture crateTexture;
	Material quadMaterial;

	void Start()
	{
		F32 vertexList[] = {
			-0.5f,0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,-0.5f,
			0.5f,0.5f,-0.5f,

			-0.5f,0.5f,0.5f,
			-0.5f,-0.5f,0.5f,
			0.5f,-0.5f,0.5f,
			0.5f,0.5f,0.5f,

			0.5f,0.5f,-0.5f,
			0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,0.5f,
			0.5f,0.5f,0.5f,

			-0.5f,0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			-0.5f,-0.5f,0.5f,
			-0.5f,0.5f,0.5f,

			-0.5f,0.5f,0.5f,
			-0.5f,0.5f,-0.5f,
			0.5f,0.5f,-0.5f,
			0.5f,0.5f,0.5f,

			-0.5f,-0.5f,0.5f,
			-0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,-0.5f,
			0.5f,-0.5f,0.5f
		};
		F32 uvCoordList[] = {
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0,
			0,0,
			0,1,
			1,1,
			1,0
		};
		U32 indexList[] = {
			0,1,3,
			3,1,2,
			4,5,7,
			7,5,6,
			8,9,11,
			11,9,10,
			12,13,15,
			15,13,14,
			16,17,19,
			19,17,18,
			20,21,23,
			23,21,22
		};

		quadModel = Loader::LoadMesh(vertexList, sizeof(vertexList), uvCoordList, sizeof(uvCoordList), indexList, sizeof(indexList));
		crateTexture = Loader::LoadTexture("Data/crate.png");
		
		quadMaterial.diffuseMap.texture = &crateTexture;
		
		quadObject = GameObject(quadModel, quadMaterial);
		quadObject.transform.SetPosition(0, 0, -2);
	}
	void Dispose()
	{
		quadModel.Dispose();
		crateTexture.Dispose();
	}
	
	void Update()
	{
		if(Input::GetKeyUp(SDL_SCANCODE_ESCAPE))
		{
			Debug::LogInfo("Escaped!");
			Exit();
		}

		quadObject.transform.Rotate(0, 1, 1);

		F32 moveSpeed = 0.01;
		F32 rotationAngle = 1;
		glm::vec3 translation;
		glm::vec3 rotationAxis;

		if(Input::GetKey(SDL_SCANCODE_W))
			translation.z -= moveSpeed;
		if(Input::GetKey(SDL_SCANCODE_S))
			translation.z += moveSpeed;
		if(Input::GetKey(SDL_SCANCODE_D))
			translation.x += moveSpeed;
		if(Input::GetKey(SDL_SCANCODE_A))
			translation.x -= moveSpeed;
		if(Input::GetKey(SDL_SCANCODE_LSHIFT))
			translation.y += moveSpeed;
		if(Input::GetKey(SDL_SCANCODE_LCTRL))
			translation.y -= moveSpeed;

		if(Input::GetKey(SDL_SCANCODE_KP_8))
			rotationAxis.x -= 1;
		if(Input::GetKey(SDL_SCANCODE_KP_2))
			rotationAxis.x += 1;
		if(Input::GetKey(SDL_SCANCODE_KP_6))
			rotationAxis.y += 1;
		if(Input::GetKey(SDL_SCANCODE_KP_4))
			rotationAxis.y -= 1;
		if(Input::GetKey(SDL_SCANCODE_KP_7))
			rotationAxis.z += 1;
		if(Input::GetKey(SDL_SCANCODE_KP_9))
			rotationAxis.z -= 1;

		camera.transform.Translate(translation);
		camera.transform.Rotate(rotationAxis, rotationAngle);
	}
	
	void Draw()
	{
		quadObject.Render(camera);
	}
	
};

RegisterGameClass(Demo);
