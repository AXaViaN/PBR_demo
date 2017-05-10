/**
 *	File: AXengine/Entity/Scene.h
 *	Purpose: Provide information about scene for shaders
 */

#ifndef __AX__ENTITY__SCENE_H
#define __AX__ENTITY__SCENE_H

#include "AXengine/Entity/Camera.h"
#include "AXengine/Entity/Light.h"
#include "AXengine/Tool/Utility.h"
#include <vector>

namespace AX { namespace Entity {

class Scene {
public:
	Scene() : camera(nullptr) { }

	void ResetScene() { _lightList.clear(); camera=nullptr; }
	void AddLight(const Light* light) { _lightList.push_back(light); }
	void AddLight(const Light**& lightList, Tool::U32 lightCount) { for( int i=0 ; i<lightCount ; i++ ) _lightList.push_back(lightList[i]); }
	
	const std::vector<const Light*>& GetLightList() const { return _lightList; }

	const Camera* camera;
	glm::mat4 projectionMatrix;

private:
	std::vector<const Light*> _lightList;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__SCENE_H
