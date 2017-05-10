#include "AXengine/Entity/EnvironmentProbe.h"

#include "AXengine/Core/Engine.h"
#include "AXengine/Gfx/FrameBuffer.h"
#include "AXengine/Gfx/Renderer.h"
#include "AXengine/Gfx/SkyboxRenderer.h"
#include "AXengine/Shader/ConvolutionShader.h"
#include "AXengine/Tool/Debug.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

namespace AX { namespace Entity {

glm::mat4 EnvironmentProbe::_projectionMatrix(glm::perspective(glm::radians(90.0f), 1.0f, Gfx::Renderer::NEAR_PLANE, Gfx::Renderer::FAR_PLANE));
Tool::U16 EnvironmentProbe::_convolutedFrameSize = 32;
Shader::ShaderProgram* EnvironmentProbe::_convolutionShader = nullptr;

void EnvironmentProbe::Init(Tool::U32 frameSize)
{
	_frameSize = frameSize;
	
	_environmentTexture = createCubemapTexture(glm::ivec2(_frameSize, _frameSize));
	_convolutedTexture = createCubemapTexture(glm::ivec2(_convolutedFrameSize, _convolutedFrameSize));
	
	_environmentMap.material.diffuseMap.texture = &_environmentTexture;
	_environmentMap.material.reflectionMap.texture = &_convolutedTexture;
}
void EnvironmentProbe::Dispose()
{
	_environmentTexture.Dispose();
	_convolutedTexture.Dispose();
}

bool EnvironmentProbe::InitConvolutionShader(Tool::F32 sampleDelta)
{
	_convolutionShader = new Shader::ConvolutionShader();

	bool initResult = static_cast<Shader::ConvolutionShader*>(_convolutionShader)->Init(sampleDelta);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("ConvolutionShader cannot be initialized!");
		return false;
	}

	return true;
}
void EnvironmentProbe::TerminateConvolutionShader()
{
	static_cast<Shader::ConvolutionShader*>(_convolutionShader)->Terminate();
	delete _convolutionShader;
}

void EnvironmentProbe::Capture(void(*RenderSceneCallback)(void*), void* callbackParam)
{
	// Set 90 FOV projection
	Gfx::Renderer::SetSceneProjection(_projectionMatrix);
	
	Gfx::FrameBuffer captureFBO;
	std::vector<glm::vec3> cameraRotation{
		glm::vec3(  0, -90, 180),	// Right
		glm::vec3(  0,  90, 180),	// Left
		glm::vec3( 90, 180, 180),	// Top
		glm::vec3(-90, 180, 180),	// Bottom
		glm::vec3(  0, 180, 180),	// Back
		glm::vec3(  0,   0, 180)	// Front
	};

	// Capture
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

	// Convolute
	if(_convolutionShader != nullptr)
	{
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
	
	// Restore
	Gfx::FrameBuffer::UseDefault();
	Gfx::Renderer::SetSceneProjection(Gfx::Renderer::GetDefaultProjectionMatrix());
}

/***** PRIVATE *****/

Asset::Texture EnvironmentProbe::createCubemapTexture(glm::ivec2 dimensions)
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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return Asset::Texture(textureID);
}

} } // namespace AX::Entity
