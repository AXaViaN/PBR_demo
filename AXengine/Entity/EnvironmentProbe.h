/**
 *	File: AXengine/Entity/EnvironmentProbe.h
 *	Purpose: Capture environment and convolute it
 *	
 *	Environment map is stored like this:
 *	material.diffuseMap.texture = captured cubemap
 *	material.reflectionMap.texture = convoluted cubemap
 *	material.environmentMap->material.diffuseMap.texture = pre-filtered cubemap
 *	material.environmentMap->material.reflectionMap.texture = BRDF integration map
 */

#ifndef __AX__ENTITY__ENVIRONMENT_PROBE_H
#define __AX__ENTITY__ENVIRONMENT_PROBE_H

#include "AXengine/Entity/Camera.h"
#include "AXengine/Entity/Cubemap.h"
#include <glm/glm.hpp>

namespace AX { namespace Entity {

class EnvironmentProbe {
public:
	void Init(Tool::U32 frameSize);
	void Dispose();
	
	static bool InitCaptureShader(Tool::F32 irradianceSampleDelta=0.025f, Tool::U32 preFilterSampleCount=1024);
	static void TerminateCaptureShader();

	void Capture(void(*RenderSceneCallback)(void*), void* callbackParam);

	void SetPosition(Tool::F32 x, Tool::F32 y, Tool::F32 z) { _captureCamera.transform.position = glm::vec3(x, y, z); }

	Cubemap& GetEnvironmentMap() { return _environmentMap; }

private:
	static Asset::Texture createCubemapTexture(glm::ivec2 dimensions, bool addMipmap);

	void captureEnvironment(void(*RenderSceneCallback)(void*), void* callbackParam, std::vector<glm::vec3> cameraRotation);
	void convoluteIrradiance(std::vector<glm::vec3> cameraRotation);
	void prefilterEnvironment(std::vector<glm::vec3> cameraRotation);

private:
	static glm::mat4 _projectionMatrix;
	static Shader::ShaderProgram* _convolutionShader;
	static Shader::ShaderProgram* _preFilterShader;
	static Tool::U16 _convolutedFrameSize;
	static Tool::U16 _prefilteredFrameSizeMax;
	static Tool::U16 _prefilteredFrameSizeMin;

	Entity::Camera _captureCamera;

	Cubemap _environmentMap;
	Cubemap _specularEnvironmentMap;

	Tool::U32 _frameSize;

	Asset::Texture _environmentTexture;
	Asset::Texture _convolutedTexture;
	Asset::Texture _prefilteredTexture;
	Asset::Texture _brdfIntegrationTexture;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__ENVIRONMENT_PROBE_H
