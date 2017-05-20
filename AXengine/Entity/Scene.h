/**
 *	File: AXengine/Entity/Scene.h
 *	Purpose: Provide information about scene for shaders
 */

#ifndef __AX__ENTITY__SCENE_H
#define __AX__ENTITY__SCENE_H

#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>
#include <vector>

namespace AX { namespace Entity {
class Camera;
class EnvironmentProbe;
class Light;
} }

namespace AX { namespace Entity {

class Scene {
public:
	Scene() : camera(nullptr) { }

	void ResetScene() { _lightList.clear(); camera=nullptr; _environmentProbeList.clear(); }
	void AddLight(const Light* light) { _lightList.push_back(light); }
	void AddLight(const Light**& lightList, Tool::U32 lightCount) { for( int i=0 ; i<lightCount ; i++ ) _lightList.push_back(lightList[i]); }
	void AddEnvironmentProbe(const EnvironmentProbe* environmentProbe) { _environmentProbeList.push_back(environmentProbe); }
	void AddEnvironmentProbe(const EnvironmentProbe**& environmentProbeList, Tool::U32 environmentProbeCount) { for( int i=0 ; i<environmentProbeCount ; i++ ) _environmentProbeList.push_back(environmentProbeList[i]); }

	const std::vector<const Light*>& GetLightList() const { return _lightList; }
	const std::vector<const EnvironmentProbe*>& GetEnvironmentProbeList() const { return _environmentProbeList; }

	const Camera* camera;
	glm::mat4 projectionMatrix;

private:
	std::vector<const Light*> _lightList;
	std::vector<const EnvironmentProbe*> _environmentProbeList;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__SCENE_H
