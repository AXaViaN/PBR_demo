#include "AXengine/Shader/ShaderProgram.h"

#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Utility.h"
#include <cstdio>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace AX { namespace Shader {

void ShaderProgram::Start() const
{
	glUseProgram(_programID);
}
void ShaderProgram::Stop() const
{
	glUseProgram(0);
}

/***** PROTECTED *****/

bool ShaderProgram::Init(const Tool::CHR* vertexFilePath, const Tool::CHR* fragmentFilePath)
{
	bool initResult;
	initResult = loadShader(vertexFilePath, GL_VERTEX_SHADER);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Vertex shader %s cannot be loaded!", vertexFilePath);
		return false;
	}
	initResult = loadShader(fragmentFilePath, GL_FRAGMENT_SHADER);
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Fragment shader %s cannot be loaded!", fragmentFilePath);
		return false;
	}

	_programID = glCreateProgram();
	if(_programID == 0)
	{
		Tool::Debug::LogWarning("OpenGL program cannot be created!");
		return false;
	}
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	// Implemented in derived class
	BindShaderAttributes();

	initResult = linkProgram();
	if(initResult == false)
	{
		Tool::Debug::LogWarning("Shaders %s and %s failed to link!", vertexFilePath, fragmentFilePath);
		return false;
	}

	// Implemented in derived class
	GetShaderUniformLocations();

	return true;
}
void ShaderProgram::Terminate()
{
	Stop();

	if(_programID)
	{
		if(_fragmentShaderID)
		{
			glDetachShader(_programID, _fragmentShaderID);
			glDeleteShader(_fragmentShaderID);
			_fragmentShaderID = 0;
		}

		if(_vertexShaderID)
		{
			glDetachShader(_programID, _vertexShaderID);
			glDeleteShader(_vertexShaderID);
			_vertexShaderID = 0;
		}

		glDeleteProgram(_programID);
		_programID = 0;
	}
	else
	{
		if(_fragmentShaderID)
		{
			glDeleteShader(_fragmentShaderID);
			_fragmentShaderID = 0;
		}

		if(_vertexShaderID)
		{
			glDeleteShader(_vertexShaderID);
			_vertexShaderID = 0;
		}
	}
}

void ShaderProgram::BindAttribute(Tool::U32 attributePosition, const Tool::CHR* variableName)
{
	glBindAttribLocation(_programID, attributePosition, variableName);
}
Tool::U32 ShaderProgram::GetUniformLocation(const Tool::CHR* uniformName)
{
	return glGetUniformLocation(_programID, uniformName);
}

void ShaderProgram::LoadUniform(const Tool::U32& uniformLocation, Tool::I32 value)
{
	glUniform1i(uniformLocation, value);
}
void ShaderProgram::LoadUniform(const Tool::U32& uniformLocation, Tool::F32 value)
{
	glUniform1f(uniformLocation, value);
}
void ShaderProgram::LoadUniform(const Tool::U32& uniformLocation, bool value)
{
	glUniform1i(uniformLocation, value);
}
void ShaderProgram::LoadUniform(const Tool::U32& uniformLocation, glm::vec2 value)
{
	glUniform2f(uniformLocation, value.x, value.y);
}
void ShaderProgram::LoadUniform(const Tool::U32& uniformLocation, glm::vec3 value)
{
	glUniform3f(uniformLocation, value.x, value.y, value.z);
}
void ShaderProgram::LoadUniform(const Tool::U32& uniformLocation, glm::mat4 value)
{
	glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(value));
}

/***** PRIVATE *****/

bool ShaderProgram::loadShader(const Tool::CHR* filePath, Tool::U32 shaderType)
{
	// Read shader code
	std::FILE* shaderFile = std::fopen(filePath, "r");
	if(shaderFile == nullptr)
	{
		Tool::Debug::LogWarning("Shader file %s cannot be opened!", filePath);
		return false;
	}

	std::fseek(shaderFile, 0, SEEK_END);
	Tool::U32 fileSize = std::ftell(shaderFile);
	std::fseek(shaderFile, 0, SEEK_SET);

	Tool::CHR* fileContent = new Tool::CHR[fileSize+1];
	std::fread(fileContent, fileSize, 1, shaderFile);
	fileContent[fileSize] = '\0';

	std::fclose(shaderFile);

	// Compile code
	GLuint shaderID = glCreateShader(shaderType);
	if(shaderID == 0)
	{
		Tool::Debug::LogWarning("OpenGL shader cannot be created! Shader type = %x", shaderType);
		return false;
	}

	glShaderSource(shaderID, 1, &fileContent, NULL);
	glCompileShader(shaderID);
	delete[] fileContent;

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		Tool::CHR* errorLog = new Tool::CHR[maxLength];
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(shaderID);

		Tool::Debug::LogWarning("%s", errorLog);
		Tool::Debug::LogWarning("Shader %s failed to compile", filePath);

		delete[] errorLog;
		return false;
	}

	if(shaderType == GL_VERTEX_SHADER)
		_vertexShaderID = shaderID;
	else if(shaderType == GL_FRAGMENT_SHADER)
		_fragmentShaderID = shaderID;

	return true;
}
bool ShaderProgram::linkProgram()
{
	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		Tool::CHR* errorLog = new Tool::CHR[maxLength];
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		Terminate();

		Tool::Debug::LogWarning("%s", &errorLog[0]);

		delete[] errorLog;
		return false;
	}

	glValidateProgram(_programID);

	return true;
}

} } // namespace AX::Shader
