/**
 *	File: AXengine/Game.h
 *	Purpose: Base class for AXengine applications
 *	
 *	User must extend from this class and register
 *	derived class with RegisterGameClass() macro.
 */

#ifndef __AX__GAME_H
#define __AX__GAME_H

#include "AXengine/Entity/Camera.h"
#include "AXengine/Entity/FreeCamera.h"
#include "AXengine/Entity/GameObject.h"
#include "AXengine/Entity/DirectionalLight.h"
#include "AXengine/Entity/PointLight.h"
#include "AXengine/Entity/SpotLight.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Model/Material.h"
#include "AXengine/Model/PhongMaterial.h"
#include "AXengine/Model/Mesh.h"
#include "AXengine/Model/Texture.h"
#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Input.h"
#include "AXengine/Tool/Loader.h"
#include "AXengine/Tool/Time.h"
#include "AXengine/Tool/Utility.h"

namespace AX {

class Game {
public:
	virtual ~Game();

	// Start is called before game loop
	virtual void Start() = 0;
	// Dispose is called after game loop
	virtual void Dispose() = 0;
	// Update is called every frame
	virtual void Update() = 0;
	// Draw is called every frame
	virtual void Draw() = 0;
	
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
