/**
 *	File: AXengine/Core/Engine.h
 *	Purpose: Init and Terminate subsystems and handle the game loop
 */

#ifndef __AX__CORE__ENGINE_H
#define __AX__CORE__ENGINE_H

#include "AXengine/Tool/Singleton.h"

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

private:
	Game* _game;
	bool _isRunning;
	
};

} } // namespace AX::Core

#endif // __AX__CORE__ENGINE_H
