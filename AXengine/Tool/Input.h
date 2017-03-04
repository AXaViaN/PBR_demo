/**
 *	File: AXengine/Tool/Input.h
 *	Purpose: Managing events to get input
 */

#ifndef __AX__TOOL__INPUT_H
#define __AX__TOOL__INPUT_H

#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <SDL2/SDL_scancode.h>
#include <glm/glm.hpp>

namespace AX { namespace Core {
class Engine;
} }

namespace AX { namespace Tool {

class Input : public Singleton<Input> {
public:
	enum MouseButton {
		LEFT,
		MIDDLE,
		RIGHT,

		NUM_MOUSE_BUTTONS
	};

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
	
	static bool GetMouseButton(MouseButton mouseButton);
	static bool GetMouseButtonDown(MouseButton mouseButton);
	static bool GetMouseButtonUp(MouseButton mouseButton);

	/**
	 * Get delta mouse movement since last frame.
	 * 
	 * It will return zero if mouse motion is inactive.
	 * @returns +x for right, -x for left, +y for down and -y for up
	 */
	static const glm::ivec2& GetMouseDelta() { return instance->_mouseDelta; }

	/**
	 * If mouse motion is active; motion info will be processed, 
	 * mouse cursor will be hidden and warped in the middle of the window.
	 */
	static void ActivateMouseMotion(bool isActive);

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
	glm::ivec2 _windowCenter;
	glm::ivec2 _mouseDelta;
	bool _isMouseActive;
	bool _hasFocus;
	U8 _mouseState[NUM_MOUSE_BUTTONS];

	U8 _keyState[SDL_NUM_SCANCODES];
	bool _isWindowClosed;
	
};

} } // namespace AX::Tool

#endif // __AX__TOOL__INPUT_H
