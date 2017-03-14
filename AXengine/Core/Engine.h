/**
 *	File: AXengine/Core/Engine.h
 *	Purpose: Init and Terminate subsystems and handle the game loop
 */

#ifndef __AX__CORE__ENGINE_H
#define __AX__CORE__ENGINE_H

#include "AXengine/Shader/StandardShader.h"
#include "AXengine/Shader/StandardShader2D.h"
#include "AXengine/Shader/PhongShader.h"
#include "AXengine/Shader/KernelShader.h"
#include "AXengine/Shader/TextShader.h"
#include "AXengine/Shader/SkyboxShader.h"
#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"

namespace AX {
class Game;
}

namespace AX { namespace Core {

class Engine : public Tool::Singleton<Engine> {
public:
	Engine() : _game(nullptr), _isRunning(false) { }
	
	/**
	 * Initialize subsystems
	 * 
	 * @param game Handle for game
	 * @returns true on success
	 */
	bool Init(Game* game);
	/**
	 * Terminate subsystems
	 * 
	 * Should be called even if the Init() is failed
	 */
	void Terminate();
	
	/**
	 * Start the game loop
	 */
	void Run();
	/**
	 * Stop the game loop
	 */
	void Exit() { _isRunning = false; }
	
	Shader::StandardShader standardShader;
	Shader::StandardShader2D standardShader2D;
	Shader::PhongShader phongShader;
	Shader::KernelShader kernelShader;
	Shader::TextShader textShader;
	Shader::SkyboxShader skyboxShader;

	const Tool::CHR* name = "PBR Demo Engine";

private:
	Game* _game;
	bool _isRunning;
	
};

} } // namespace AX::Core

#endif // __AX__CORE__ENGINE_H
