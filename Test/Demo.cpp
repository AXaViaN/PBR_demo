#include "AXengine/Game.h"

using namespace AX::Gfx;
using namespace AX::Model;
using namespace AX::Tool;

class Demo : public AX::Game {
public:
	Mesh quad;

	void Start()
	{
		F32 vertexList[] = {
			-0.5f,  0.5f,  0.0f,
			-0.5f, -0.5f,  0.0f,
			 0.5f,  0.5f,  0.0f,
			 0.5f, -0.5f,  0.0f
		};
		U32 indexList[] = {
			0, 1, 2,
			2, 1, 3
		};

		quad = Loader::LoadMesh(vertexList, sizeof(vertexList), indexList, sizeof(indexList));
	}
	void Dispose()
	{
		quad.Dispose();
	}
	
	void Update()
	{
		if(Input::GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			Debug::LogInfo("Escaped!");
			Exit();
		}
	}
	
	void Draw()
	{
		Renderer::Render(quad);
	}
	
};

RegisterGameClass(Demo);
