/**
 *	File: AXengine/Tool/Input.h
 *	Purpose: Managing events to get input
 */

#ifndef __AX__TOOL__INPUT_H
#define __AX__TOOL__INPUT_H

#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <SDL2/SDL_scancode.h>

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Tool {

class Input : public Singleton<Input> {
public:
	/**
	 * @param SDLscancode Scan code for key in SDL_SCANCODE_x format
	 * @returns true if the key is holding
	 */
	static bool GetKey(U32 SDLscancode);
	/**
	 * @param SDLscancode Scan code for key in SDL_SCANCODE_x format
	 * @returns true if the key is down
	 */
	static bool GetKeyDown(U32 SDLscancode);
	/**
	 * @param SDLscancode Scan code for key in SDL_SCANCODE_x format
	 * @returns true if the key is up
	 */
	static bool GetKeyUp(U32 SDLscancode);
	
protected:
	/**
	 * Init and update is only visible for Engine
	 * 
	 * This is done to prevent API users to call these methods and
	 * get UB.
	 */
	friend class Core::Engine;
	void Init();
	void Update();
	bool IsWindowClosed() { return _isWindowClosed; }

private:
	U8 _keyState[SDL_NUM_SCANCODES];
	bool _isWindowClosed;
	
};

} } // namespace AX::Tool

#endif // __AX__TOOL__INPUT_H
