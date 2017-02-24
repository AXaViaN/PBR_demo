#include "AXengine/Core/Window.h"

#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Time.h"
#include <GL/glew.h>

namespace AX { namespace Core {

/*	Create window and initialize graphics context		*/
bool Window::Create(const Tool::CHR* title, const Tool::U32 width, const Tool::U32 height)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Tool::Debug::LogWarning("SDL init failed: %s", SDL_GetError());
		return false;
	}
	
	_handle = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if(_handle == nullptr)
	{
		Tool::Debug::LogWarning("Window creation failed: %s", SDL_GetError());
		return false;
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	_glContext = SDL_GL_CreateContext(_handle);
	if(_glContext == nullptr)
	{
		Tool::Debug::LogWarning("OpenGL context creation failed: %s", SDL_GetError());
		return false;
	}
	
	glewExperimental = GL_TRUE;
	GLenum glewInitResult = glewInit();
	if(glewInitResult != GLEW_OK)
	{
		Tool::Debug::LogWarning("GLEW init failed: %s", glewGetErrorString(glewInitResult));
		return false;
	}
	
	// Init timer
	Tool::Time::Instance();

	return true;
}
/*	Destroy window and terminate graphics context		*/
void Window::Destroy()
{
	if(_glContext)
	{
		SDL_GL_DeleteContext(_glContext);
		_glContext = nullptr;
	}
	
	if(_handle)
	{
		SDL_DestroyWindow(_handle);
		_handle = nullptr;
	}
	
	SDL_Quit();
}

void Window::RenderPresent() const
{
	SDL_GL_SwapWindow(_handle);
}
void Window::SyncFPS(Tool::U32 fps) const
{
	static Tool::U32 lastFrame = 0;
	
	if(1000/fps > SDL_GetTicks()-lastFrame)
		SDL_Delay(1000/fps - (SDL_GetTicks()-lastFrame));
	
	Tool::Time::Instance().deltaTime = (SDL_GetTicks() - lastFrame) / 1000.0f;
	lastFrame = SDL_GetTicks();
}

glm::ivec2 Window::GetWindowSize()
{
	Tool::I32 width, height;
	SDL_GetWindowSize(instance->_handle, &width, &height);

	return glm::ivec2(width, height);
}

glm::ivec2 Window::WarpMouse()
{
	glm::ivec2 window = GetWindowSize() / 2;
	SDL_WarpMouseInWindow(_handle, window.x, window.y);

	return window;
}

} } // namespace AX::Core
