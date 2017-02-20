#include "AXengine/Tool/Input.h"

#include "AXengine/Core/Window.h"
#include <SDL2/SDL.h>

namespace AX { namespace Tool {

enum KeyState {
	Idle,
	Down,
	Up,
	Hold
};

bool Input::GetKey(U32 SDLscancode)
{
	return instance->_keyState[SDLscancode] == KeyState::Hold || GetKeyDown(SDLscancode);
}
bool Input::GetKeyDown(U32 SDLscancode)
{
	return instance->_keyState[SDLscancode] == KeyState::Down;
}
bool Input::GetKeyUp(U32 SDLscancode)
{
	return instance->_keyState[SDLscancode] == KeyState::Up;
}

bool Input::GetMouseButton(MouseButton mouseButton)
{
	return instance->_mouseState[mouseButton] == KeyState::Hold || GetMouseButtonDown(mouseButton);
}
bool Input::GetMouseButtonDown(MouseButton mouseButton)
{
	return instance->_mouseState[mouseButton] == KeyState::Down;
}
bool Input::GetMouseButtonUp(MouseButton mouseButton)
{
	return instance->_mouseState[mouseButton] == KeyState::Up;
}

void Input::ActivateMouseMotion(bool isActive)
{
	instance->_isMouseActive = isActive;

	SDL_ShowCursor(!isActive);

	if(isActive)
		instance->_windowCenter = Core::Window::GetWindowSize() / 2;
	else
		instance->_mouseDelta = glm::ivec2();
}

/***** PROTECTED *****/

void Input::Init()
{
	for( U32 key=0 ; key<sizeof(_keyState) ; key++ )
		_keyState[key] = KeyState::Idle;

	for( U32 button=0 ; button<sizeof(_mouseState) ; button++ )
		_mouseState[button] = KeyState::Idle;

	_isWindowClosed = false;
	_isMouseActive = false;
}
void Input::Update()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
			{
				_isWindowClosed = true;
				break;
			}
		}
	}

	const U8* keyboardState = SDL_GetKeyboardState(NULL);
	for( U32 key=0 ; key<sizeof(_keyState) ; key++ )
	{
		if(keyboardState[key] == true)
		{
			if(_keyState[key] == KeyState::Down)
				_keyState[key] = KeyState::Hold;
			else if(_keyState[key] != KeyState::Hold)
				_keyState[key] = KeyState::Down;
		}
		else
		{
			if(_keyState[key] == KeyState::Down || _keyState[key] == KeyState::Hold)
				_keyState[key] = KeyState::Up;
			else
				_keyState[key] = KeyState::Idle;
		}
	}

	U32 mouseButtonState = SDL_GetMouseState(nullptr, nullptr);
	for( U32 button=0 ; button<sizeof(_mouseState) ; button++ )
	{
		if(mouseButtonState & SDL_BUTTON(button+1))
		{
			if(_mouseState[button] == KeyState::Down)
				_mouseState[button] = KeyState::Hold;
			else if(_mouseState[button] != KeyState::Hold)
				_mouseState[button] = KeyState::Down;
		}
		else
		{
			if(_mouseState[button] == KeyState::Down || _mouseState[button] == KeyState::Hold)
				_mouseState[button] = KeyState::Up;
			else
				_mouseState[button] = KeyState::Idle;
		}
	}

	if(_isMouseActive)
	{
		glm::ivec2 newMousePosition;
		SDL_GetMouseState(&newMousePosition.x, &newMousePosition.y);

		_mouseDelta = newMousePosition - _windowCenter;

		_windowCenter = Core::Window::Instance().WarpMouse();
	}
}

} } // namespace AX::Tool
