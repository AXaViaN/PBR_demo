#include "AXengine/Game.h"

#include "AXengine/Core/Engine.h"

namespace AX {

void Game::Run()
{
	// Initialize and run the engine
	bool initResult = Core::Engine::Instance().Init(this);
	if(initResult == true)
		Core::Engine::Instance().Run();
}
void Game::Exit()
{
	// Exit from game loop
	Core::Engine::Instance().Exit();
}

/***** PROTECTED *****/

Game::~Game()
{
	// Termination should be called with a separate method but
	// it wasn't safe. It could be called while inside  game loop.
	Core::Engine::Instance().Terminate();
}

} // namespace AX
