/**
 *	File: AXengine/Entity/EnvironmentProbe.h
 *	Purpose: Capture environment and convolute it
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
	
	static bool InitConvolutionShader(Tool::F32 sampleDelta=0.025f);
	static void TerminateConvolutionShader();

	void Capture(void(*RenderSceneCallback)(void*), void* callbackParam);

	void SetPosition(Tool::F32 x, Tool::F32 y, Tool::F32 z) { _captureCamera.transform.position = glm::vec3(x, y, z); }

	Cubemap& GetEnvironmentMap() { return _environmentMap; }

private:
	static Asset::Texture createCubemapTexture(glm::ivec2 dimensions);

private:
	static glm::mat4 _projectionMatrix;
	static Tool::U16 _convolutedFrameSize;
	static Shader::ShaderProgram* _convolutionShader;

	Entity::Camera _captureCamera;

	Cubemap _environmentMap;

	Tool::U32 _frameSize;

	Asset::Texture _environmentTexture;
	Asset::Texture _convolutedTexture;

};

} } // namespace AX::Entity

#endif // __AX__ENTITY__ENVIRONMENT_PROBE_H
