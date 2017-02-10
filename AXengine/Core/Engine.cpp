#include "AXengine/Core/Engine.h"

#include "AXengine/Game.h"
#include "AXengine/Core/Window.h"
#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Input.h"

namespace AX { namespace Core {

/*	Initialize subsystems	*/
bool Engine::Init(Game* game)
{
	if(_game)
	{
		Tool::Debug::LogWarning("Engine is already initialized!");
		return false;
	}
	_game = game;
	
	Tool::Debug::SetLogTarget("debug.log", Engine::Name);

	// Init window
	bool initResult = Window::Instance().Create(Engine::Name, 1280, 720);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Window couldn't created!");
		return false;
	}
	Tool::Debug::LogInfo("Window created!");
	
	Tool::Input::Instance().Init();

	return true;
}
/*	Terminate subsystems		*/
void Engine::Terminate()
{
	Window::Instance().Destroy();
	
	Tool::Debug::CloseLogTarget();

	_game = nullptr;
}

/*	Start the game loop		*/
void Engine::Run()
{
	Window& window = Window::Instance();
	Tool::Input& input = Tool::Input::Instance();
	
	_game->Start();
	
	_isRunning = true;
	while(_isRunning && input.IsWindowClosed() == false)
	{
		input.Update();

		_game->Update();
		
		window.Clear(0.2f, 0.4f, 0.7f);
		
		_game->Render();
		
		window.RenderPresent();
		window.SyncFPS(60);	
	}
	
	_game->Dispose();
}

} } // namespace AX::Core
