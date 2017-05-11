#include "AXengine/Core/Engine.h"

#include "AXengine/Game.h"
#include "AXengine/Core/Window.h"
#include "AXengine/Entity/Quad.h"
#include "AXengine/Entity/Cubemap.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Gfx/Renderer2D.h"
#include "AXengine/Gfx/TextRenderer.h"
#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Input.h"
#include "AXengine/Tool/Loader.h"
#include <glm/gtx/compatibility.hpp>

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

	initResult = standardShader.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("StandardShader cannot be initialized!");
		return false;
	}

	initResult = phongShader.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("PhongShader cannot be initialized!");
		return false;
	}

	initResult = pbrShader.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("PBRShader cannot be initialized!");
		return false;
	}

	initResult = skyboxShader.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("SkyboxShader cannot be initialized!");
		return false;
	}
	
	Gfx::Renderer2D::Instance().Init();
	Entity::Quad::InitMesh();
	Entity::Cubemap::InitMesh();

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

	initResult = toneShader.Init();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("ToneShader cannot be initialized!");
		return false;
	}

	return true;
}
/*	Terminate subsystems		*/
void Engine::Terminate()
{
	Entity::Quad::DisposeMesh();
	Entity::Cubemap::DisposeMesh();

	toneShader.Terminate();
	skyboxShader.Terminate();
	kernelShader.Terminate();
	standardShader2D.Terminate();
	textShader.Terminate();
	phongShader.Terminate();
	pbrShader.Terminate();
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

	Gfx::Renderer::Clear(0.4f, 0.4f, 0.4f);
	toneShader.SetGamma(2.0f);
	bool useAutoExposure = true;
	Tool::F32 hdrExposure = 1.0f;
	
	Gfx::FrameBuffer renderBuffer;
	bool initResult = renderBuffer.Init(window.GetWindowSize(), Gfx::FrameBuffer::HDR_COLOR | Gfx::FrameBuffer::COLOR_TEXTURE__DEPTH_STENCIL_BUFFER);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Engine FrameBuffer error.");
		return;
	}

	Entity::Quad renderQuad;
	renderQuad.material.shader = &toneShader;
	renderQuad.material.diffuseMap.texture = &renderBuffer.GetColorTexture();

	_isRunning = true;
	_game->Start();
	Gfx::Renderer::PrepareScene();
	while(_isRunning && input.IsWindowClosed()==false)
	{
		input.Update();

		_game->Update();
		
		Gfx::Renderer::Clear();
		_game->Draw();
		
		renderBuffer.Use();
		Gfx::Renderer::Clear();
		Gfx::SkyboxRenderer::RenderSkybox();
		Gfx::Renderer::RenderBatch();
		Gfx::Renderer2D::RenderBatch();

		Gfx::FrameBuffer::UseDefault();
		renderQuad.RenderImmediate();

		Gfx::TextRenderer::RenderBatch();

		// Automatic exposure control
		if(useAutoExposure)
		{
			Tool::F32 averageBrightness = renderBuffer.GetAvarageBrightness();
			if(averageBrightness > 0)
			{
				Tool::F32 targetExposure = 0.5f/averageBrightness;
				Tool::F32 lerpRate = 0.1f;
				// Adapting from bright to dark is slower
				if(targetExposure > hdrExposure)
					lerpRate *= 0.25f;

				hdrExposure = glm::lerp<Tool::F32>(hdrExposure, targetExposure, lerpRate);
				hdrExposure = glm::clamp(hdrExposure, 0.0f, 3.0f);
				useAutoExposure = toneShader.SetAutoExposure(hdrExposure);
			}
		}
		
		window.RenderPresent();
		window.SyncFPS(60);
	}
	
	_game->Dispose();
}

} } // namespace AX::Core
