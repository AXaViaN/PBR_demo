/**
 *	File: AXengine/Entity/EnvironmentProbe.h
 *	Purpose: Capture environment and convolute it
 *	
 *	Environment map is stored like this:
 *	material.diffuseMap.texture = captured cubemap
 *	material.reflectionMap.texture = convoluted cubemap
 *	material.environmentMap->material.diffuseMap.texture = pre-filtered cubemap
 *	material.environmentMap->material.diffuseMap.texture.value.r = pre-filter mipmap max LOD
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
	void Init(Tool::U32 frameSize, Tool::F64 effectVolume=10);
	void Dispose();
	
	static bool InitCaptureShader(Tool::F32 irradianceSampleDelta=0.025f, Tool::U32 preFilterSampleCount=1024);
	static void TerminateCaptureShader();

	void Capture(void(*RenderSceneCallback)(void*), void* callbackParam, glm::vec3 position=glm::vec3());

	Cubemap GetEnvironmentMap() const { return _environmentMap; }
	const glm::vec3& GetPositon() const { return _captureCamera.transform.position; }
	const Tool::F64& GetEffectVolume() const { return _effectVolume; }
	static const Tool::F64& GetMaxEffectVolume() { return _maxEffectVolume; }

private:
	static Asset::Texture createCubemapTexture(glm::ivec2 dimensions, bool addMipmap);
	static Asset::Texture createBRDFIntegrationTexture(glm::ivec2 dimensions);

	void captureEnvironment(void(*RenderSceneCallback)(void*), void* callbackParam, std::vector<glm::vec3> cameraRotation);
	void convoluteIrradiance(std::vector<glm::vec3> cameraRotation);
	void prefilterEnvironment(std::vector<glm::vec3> cameraRotation);

private:
	static glm::mat4 _projectionMatrix;
	static Tool::F64 _maxEffectVolume;
	static Shader::ShaderProgram* _convolutionShader;
	static Shader::ShaderProgram* _preFilterShader;
	static Tool::U16 _convolutedFrameSize;
	static Tool::U16 _prefilteredFrameSizeMax;
	static Tool::U16 _prefilteredFrameSizeMin;

	Entity::Camera _captureCamera;

	Cubemap _environmentMap;
	Cubemap _specularEnvironmentMap;

	Tool::F64 _effectVolume;
	Tool::U32 _frameSize;

	Asset::Texture _environmentTexture;
	Asset::Texture _convolutedTexture;
	Asset::Texture _prefilteredTexture;
	
	static Asset::Texture _brdfIntegrationTexture;
	static Tool::U32 _brdfIntegrationUserCount;
	static Tool::U16 _brdfIntegrationFrameSize;
	static Tool::U16 _brdfIntegrationSampleCount;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__ENVIRONMENT_PROBE_H
