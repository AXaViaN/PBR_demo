#include "AXengine/Tool/Input.h"

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

/***** PROTECTED *****/

void Input::Init()
{
	for( U32 key=0 ; key<sizeof(_keyState) ; key++ )
		_keyState[key] = KeyState::Idle;

	_isWindowClosed = false;
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
		if(_keyState[key] == KeyState::Down)
		{
			if(keyboardState[key] == true)
				_keyState[key] = KeyState::Hold;
			else
				_keyState[key] = KeyState::Up;
		}
		else if(_keyState[key] == KeyState::Up)
		{
			if(keyboardState[key] == true)
				_keyState[key] = KeyState::Down;
			else
				_keyState[key] = KeyState::Idle;
		}
		else if(_keyState[key] == KeyState::Hold)
		{
			if(keyboardState[key] == false)
				_keyState[key] = KeyState::Up;
		}
		else
		{
			if(keyboardState[key] == true)
				_keyState[key] = KeyState::Down;
		}
	}
}

} } // namespace AX::Tool
