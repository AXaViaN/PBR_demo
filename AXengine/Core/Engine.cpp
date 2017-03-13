#include "AXengine/Core/Engine.h"

#include "AXengine/Game.h"
#include "AXengine/Core/Window.h"
#include "AXengine/Entity/Quad.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Gfx/Renderer2D.h"
#include "AXengine/Gfx/TextRenderer.h"
#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Input.h"
#include "AXengine/Tool/Loader.h"

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
	
	Tool::Debug::SetLogTarget("debug.log", Engine::name);

	// Init window
	bool initResult = Window::Instance().Create(Engine::name, 1280, 720);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Window couldn't created!");
		return false;
	}
	Tool::Debug::LogInfo("Window created!");
	
	initResult = Tool::Loader::Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Loader cannot be initialized!");
		return false;
	}

	Tool::Input::Instance().Init();
	Tool::Input::ActivateMouseMotion(true);

	Gfx::Renderer::Instance().Init();

	initResult = standardShader.Init(Gfx::Renderer::GetDefaultProjectionMatrix());
	if(initResult == false)
	{
		Tool::Debug::LogWarning("StandardShader cannot be initialized!");
		return false;
	}

	initResult = phongShader.Init(Gfx::Renderer::GetDefaultProjectionMatrix());
	if(initResult == false)
	{
		Tool::Debug::LogWarning("PhongShader cannot be initialized!");
		return false;
	}

	Gfx::Renderer2D::Instance().Init();
	Entity::Quad::InitMesh();

	glm::ivec2 windowSize = Window::GetWindowSize();
	initResult = textShader.Init(Gfx::Renderer2D::CreateProjectionMatrix(windowSize.x, windowSize.y));
	if(initResult == false)
	{
		Tool::Debug::LogWarning("TextShader cannot be initialized!");
		return false;
	}

	initResult = standardShader2D.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("StandardShader2D cannot be initialized!");
		return false;
	}

	initResult = kernelShader.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("KernelShader cannot be initialized!");
		return false;
	}

	return true;
}
/*	Terminate subsystems		*/
void Engine::Terminate()
{
	Entity::Quad::DisposeMesh();

	kernelShader.Terminate();
	standardShader2D.Terminate();
	textShader.Terminate();
	phongShader.Terminate();
	standardShader.Terminate();

	Tool::Loader::Terminate();

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
		
		Gfx::Renderer::Clear(0.4f, 0.4f, 0.4f);
		_game->Draw();
		Gfx::Renderer::RenderBatch();
		Gfx::Renderer2D::RenderBatch();
		Gfx::TextRenderer::RenderBatch();
		
		window.RenderPresent();
		window.SyncFPS(60);
	}
	
	_game->Dispose();
}

} } // namespace AX::Core
