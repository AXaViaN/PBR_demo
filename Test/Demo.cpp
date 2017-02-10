#include "AXengine/Game.h"

using namespace AX::Tool;

class Demo : public AX::Game {
public:
	void Start()
	{

	}
	void Dispose()
	{

	}
	
	void Update()
	{
		if(Input::GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			Debug::LogInfo("Escaped!");
			Exit();
		}
	}
	
	void Render()
	{

	}
	
};

RegisterGameClass(Demo);
