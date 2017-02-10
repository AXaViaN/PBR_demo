/**
 *	File: AXengine/Game.h
 *	Purpose: Base class for AXengine applications
 *	
 *	User must extend from this class and register
 *	derived class with RegisterGameClass() macro.
 */

#ifndef __AX__GAME_H
#define __AX__GAME_H

#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Input.h"

namespace AX {

class Game {
public:
	// Start is called before game loop
	virtual void Start() = 0;
	// Dispose is called after game loop
	virtual void Dispose() = 0;
	// Update is called every frame
	virtual void Update() = 0;
	// Render is called every frame
	virtual void Render() = 0;
	
	/**
	 * Run should be called from program entry point
	 * 
	 * It is done automatically if you registered the
	 * game class with RegisterGameClass()
	 */
	void Run();
	/**
	 * Exit will close the application after the frame finishes
	 */
	void Exit();
	
protected:
	virtual ~Game();
	
};

} // namespace AX

// RegisterGameClass defines the program entry point
#define RegisterGameClass(GameClass) \
	int main(int argc, const char* argv[]) \
	{ \
		GameClass game; \
		game.Run(); \
		return 0; \
	}

#endif // __AX__GAME_H
