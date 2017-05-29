#include "AXengine/Entity/EnvironmentProbe.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Entity/Quad.h"
#include "AXengine/Gfx/FrameBuffer.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Gfx/SkyboxRenderer.h"
#include "AXengine/Shader/BRDFIntegrationShader.h"
#include "AXengine/Shader/ConvolutionShader.h"
#include "AXengine/Shader/PreFilterShader.h"
#include "AXengine/Tool/Debug.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

namespace AX { namespace Entity {

glm::mat4 EnvironmentProbe::_projectionMatrix(glm::perspective(glm::radians(90.0f), 1.0f, Gfx::Renderer::NEAR_PLANE, Gfx::Renderer::FAR_PLANE));
Tool::F64 EnvironmentProbe::_maxEffectVolume = 0;
Shader::ShaderProgram* EnvironmentProbe::_convolutionShader = nullptr;
Shader::ShaderProgram* EnvironmentProbe::_preFilterShader = nullptr;
Tool::U16 EnvironmentProbe::_convolutedFrameSize = 32;
Tool::U16 EnvironmentProbe::_prefilteredFrameSizeMax = 128;
Tool::U16 EnvironmentProbe::_prefilteredFrameSizeMin = 4;

Asset::Texture EnvironmentProbe::_brdfIntegrationTexture;
Tool::U32 EnvironmentProbe::_brdfIntegrationUserCount = 0;
Tool::U16 EnvironmentProbe::_brdfIntegrationFrameSize = 512;
Tool::U16 EnvironmentProbe::_brdfIntegrationSampleCount;

void EnvironmentProbe::Init(Tool::U32 frameSize, Tool::F64 effectVolume)
{
	_frameSize = frameSize;
	_effectVolume = glm::abs(effectVolume);
	_effectVolume = glm::max(1.0, _effectVolume);
	if(_effectVolume > _maxEffectVolume)
		_maxEffectVolume = _effectVolume;

	_environmentMap.material.diffuseMap.value.r = _frameSize;
	
	_environmentTexture = createCubemapTexture(glm::ivec2(_frameSize, _frameSize), true);
	_convolutedTexture = createCubemapTexture(glm::ivec2(_convolutedFrameSize, _convolutedFrameSize), false);
	_prefilteredTexture = createCubemapTexture(glm::ivec2(_prefilteredFrameSizeMax, _prefilteredFrameSizeMax), true);

	_environmentMap.material.diffuseMap.texture = &_environmentTexture;
	_environmentMap.material.reflectionMap.texture = &_convolutedTexture;
	_environmentMap.material.environmentMap = &_specularEnvironmentMap;
	
	_specularEnvironmentMap.material.diffuseMap.texture = &_prefilteredTexture;
	_specularEnvironmentMap.material.reflectionMap.texture = &_brdfIntegrationTexture;

	if(_brdfIntegrationUserCount == 0)
	{
		_brdfIntegrationTexture = createBRDFIntegrationTexture(glm::ivec2(_brdfIntegrationFrameSize, _brdfIntegrationFrameSize));
		_brdfIntegrationUserCount++;
	}
}
void EnvironmentProbe::Dispose()
{
	_environmentTexture.Dispose();
	_convolutedTexture.Dispose();
	_prefilteredTexture.Dispose();

	if(_brdfIntegrationUserCount > 0)
		_brdfIntegrationUserCount--;
	if(_brdfIntegrationUserCount == 0)
		_brdfIntegrationTexture.Dispose();
}

bool EnvironmentProbe::InitCaptureShader(Tool::F32 irradianceSampleDelta, Tool::U32 preFilterSampleCount)
{
	_brdfIntegrationSampleCount = preFilterSampleCount;

	_convolutionShader = new Shader::ConvolutionShader();
	_preFilterShader = new Shader::PreFilterShader();

	bool initResult = static_cast<Shader::ConvolutionShader*>(_convolutionShader)->Init(irradianceSampleDelta);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("ConvolutionShader cannot be initialized!");
		return false;
	}

	initResult = static_cast<Shader::PreFilterShader*>(_preFilterShader)->Init(preFilterSampleCount);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("PreFilterShader cannot be initialized!");
		return false;
	}

	return true;
}
void EnvironmentProbe::TerminateCaptureShader()
{
	static_cast<Shader::PreFilterShader*>(_preFilterShader)->Terminate();
	static_cast<Shader::ConvolutionShader*>(_convolutionShader)->Terminate();

	delete _preFilterShader;
	delete _convolutionShader;
}

void EnvironmentProbe::Capture(void(*RenderSceneCallback)(void*), void* callbackParam, glm::vec3 position)
{
	// Set 90 FOV projection
	Gfx::Renderer::SetSceneProjection(_projectionMatrix);
	
	_captureCamera.transform.position = position;
	std::vector<glm::vec3> cameraRotation{
		glm::vec3(  0, -90, 180),	// Right
		glm::vec3(  0,  90, 180),	// Left
		glm::vec3( 90, 180, 180),	// Top
		glm::vec3(-90, 180, 180),	// Bottom
		glm::vec3(  0, 180, 180),	// Back
		glm::vec3(  0,   0, 180)	// Front
	};

	captureEnvironment(RenderSceneCallback, callbackParam, cameraRotation);

	if(_convolutionShader != nullptr)
		convoluteIrradiance(cameraRotation);

	if(_preFilterShader != nullptr)
		prefilterEnvironment(cameraRotation);
	
	// Restore
	Gfx::FrameBuffer::UseDefault();
	Gfx::Renderer::SetSceneProjection(Gfx::Renderer::GetDefaultProjectionMatrix());
}

/***** PRIVATE *****/

Asset::Texture EnvironmentProbe::createCubemapTexture(glm::ivec2 dimensions, bool addMipmap)
{
	Tool::U32 textureID;
	glGenTextures(1, &textureID);
	if(textureID == 0)
	{
		Tool::Debug::LogWarning("OpenGL texture generator failed!");
		return Asset::Texture(0);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for( Tool::SIZE i=0 ; i<6 ; i++ )
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, dimensions.x, dimensions.y, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if(addMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return Asset::Texture(textureID);
}
Asset::Texture EnvironmentProbe::createBRDFIntegrationTexture(glm::ivec2 dimensions)
{
	// Init shader
	Shader::BRDFIntegrationShader brdfIntegrationShader;
	bool initResult = brdfIntegrationShader.Init(_brdfIntegrationSampleCount);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("BRDFIntegrationShader cannot be initialized!");
		return Asset::Texture(0);
	}

	// Create empty texture
	Tool::U32 textureID;
	glGenTextures(1, &textureID);
	if(textureID == 0)
	{
		Tool::Debug::LogWarning("OpenGL texture generator failed!");
		brdfIntegrationShader.Terminate();
		return Asset::Texture(0);
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, dimensions.x, dimensions.y, 0, GL_RG, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Fill texture
	Quad quad;
	quad.material.shader = &brdfIntegrationShader;

	Gfx::FrameBuffer FBO;
	FBO.Init(glm::ivec2(dimensions.x, dimensions.y), Gfx::FrameBuffer::DEPTH_STENCIL_BUFFER);
	FBO.Use();

	FBO.SetColorTexture(textureID);

	Gfx::Renderer::PrepareScene();
	Gfx::Renderer::Clear();
	quad.RenderImmediate();
	
	FBO.Terminate();
	Gfx::FrameBuffer::UseDefault();
	brdfIntegrationShader.Terminate();

	return Asset::Texture(textureID);
}

void EnvironmentProbe::captureEnvironment(void(*RenderSceneCallback)(void*), void* callbackParam, std::vector<glm::vec3> cameraRotation)
{
	Gfx::FrameBuffer captureFBO;
	captureFBO.Init(glm::ivec2(_frameSize, _frameSize), Gfx::FrameBuffer::DEPTH_STENCIL_BUFFER);
	captureFBO.Use();
	for( Tool::SIZE i=0 ; i<6 ; i++ )
	{
		captureFBO.SetColorTexture(_environmentTexture.GetTextureID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X+i);

		Gfx::Renderer::Clear();
		Gfx::Renderer::PrepareScene();
		RenderSceneCallback(callbackParam);

		_captureCamera.transform.rotation = cameraRotation[i];
		Gfx::Renderer::SetSceneCamera(&_captureCamera);

		Gfx::Renderer::Clear();
		Gfx::SkyboxRenderer::RenderSkybox();
		Gfx::Renderer::RenderBatch();
	}
	captureFBO.Terminate();

	// Generate mipmaps for pre-filtering adjustments
	glBindTexture(GL_TEXTURE_CUBE_MAP, _environmentTexture.GetTextureID());
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
void EnvironmentProbe::convoluteIrradiance(std::vector<glm::vec3> cameraRotation)
{
	Gfx::FrameBuffer captureFBO;
	captureFBO.Init(glm::ivec2(_convolutedFrameSize, _convolutedFrameSize), Gfx::FrameBuffer::DEPTH_STENCIL_BUFFER);
	captureFBO.Use();
	Shader::ShaderProgram* environmentShader = _environmentMap.material.shader;
	_environmentMap.material.shader = _convolutionShader;
	for( Tool::SIZE i=0 ; i<6 ; i++ )
	{
		captureFBO.SetColorTexture(_convolutedTexture.GetTextureID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X+i);

		Gfx::Renderer::PrepareScene();
		Gfx::SkyboxRenderer::Render(_environmentMap);

		_captureCamera.transform.rotation = cameraRotation[i];
		Gfx::Renderer::SetSceneCamera(&_captureCamera);

		Gfx::Renderer::Clear();
		Gfx::SkyboxRenderer::RenderSkybox();
	}
	captureFBO.Terminate();
	_environmentMap.material.shader = environmentShader;
}
void EnvironmentProbe::prefilterEnvironment(std::vector<glm::vec3> cameraRotation)
{
	Shader::ShaderProgram* environmentShader = _environmentMap.material.shader;
	_environmentMap.material.shader = _preFilterShader;
	Tool::U16 mipmapCount = glm::log2(static_cast<Tool::F32>(_prefilteredFrameSizeMax)/static_cast<Tool::F32>(_prefilteredFrameSizeMin));
	_specularEnvironmentMap.material.diffuseMap.value.r = mipmapCount-1;
	for( Tool::U16 mipmapLevel=0 ; mipmapLevel<mipmapCount ; mipmapLevel++ )
	{
		glm::ivec2 frameSize = glm::ivec2(_prefilteredFrameSizeMax, _prefilteredFrameSizeMax);
		frameSize.x *= glm::pow(0.5f, mipmapLevel);
		frameSize.y *= glm::pow(0.5f, mipmapLevel);

		float roughness = static_cast<Tool::F32>(mipmapLevel) / static_cast<Tool::F32>(mipmapCount-1);
		_environmentMap.material.reflectionMap.value = roughness;

		Gfx::FrameBuffer captureFBO;
		captureFBO.Init(frameSize, Gfx::FrameBuffer::DEPTH_STENCIL_BUFFER);
		captureFBO.Use();
		for( Tool::SIZE i=0 ; i<6 ; i++ )
		{
			captureFBO.SetColorTexture(_prefilteredTexture.GetTextureID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, mipmapLevel);

			Gfx::Renderer::PrepareScene();
			Gfx::SkyboxRenderer::Render(_environmentMap);

			_captureCamera.transform.rotation = cameraRotation[i];
			Gfx::Renderer::SetSceneCamera(&_captureCamera);

			Gfx::Renderer::Clear();
			Gfx::SkyboxRenderer::RenderSkybox();
		}
		captureFBO.Terminate();
	}
	_environmentMap.material.reflectionMap.value = 0.0f;
	_environmentMap.material.shader = environmentShader;
}

} } // namespace AX::Entity
