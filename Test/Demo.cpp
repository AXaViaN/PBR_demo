#include "AXengine/Game.h"

class Demo : public AX::Game {
public:
	void Start()
	{
		AX::Tool::Debug::LogInfo("Game started!");
	}
	void Dispose()
	{
		AX::Tool::Debug::LogInfo("Game disposed!");
	}
	
	void Update()
	{
		AX::Tool::Debug::LogInfo("Game updated!");
	}
	
	void Render()
	{
		AX::Tool::Debug::LogInfo("Game rendered!");
	}
	
};

RegisterGameClass(Demo);
