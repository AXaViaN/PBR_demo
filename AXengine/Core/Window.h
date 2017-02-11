/**
 *	File: AXengine/Core/Window.h
 *	Purpose: Handle application window and graphics context
 */

#ifndef __AX__CORE__WINDOW_H
#define __AX__CORE__WINDOW_H

#include "AXengine/Tool/Singleton.h"
#include "AXengine/Tool/Utility.h"
#include <SDL2/SDL.h>

namespace AX { namespace Core {

class Window : public Tool::Singleton<Window> {
public:
	Window() : _handle(nullptr), _glContext(nullptr) { }
	
	/**
	 * Create window and initialize graphics context
	 * 
	 * @returns true on success
	 */
	bool Create(const Tool::CHR* title, const Tool::U32 width, const Tool::U32 height);
	/**
	 * Destroy window and terminate graphics context
	 */
	void Destroy();
	
	void RenderPresent();
	void SyncFPS(Tool::U32 fps);
	
private:
	SDL_Window* _handle;
	SDL_GLContext _glContext;
	
};

} } // namespace AX::Core

#endif // __AX__CORE__WINDOW_H
