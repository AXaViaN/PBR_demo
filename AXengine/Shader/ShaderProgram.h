/**
 *	File: AXengine/Shader/ShaderProgram.h
 *	Purpose: Base class for shaders
 */

#ifndef __AX__SHADER__SHADER_PROGRAM_H
#define __AX__SHADER__SHADER_PROGRAM_H

#include "AXengine/Model/Material.h"
#include "AXengine/Tool/Utility.h"
#include <glm/glm.hpp>

namespace AX { namespace Shader {

class ShaderProgram {
public:
	ShaderProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0) { }
	virtual ~ShaderProgram() { }
	
	/**
	 * Start shader program. Typically called before rendering.
	 */
	void Start();
	/**
	 * Stop shader program. Typically called after rendering.
	 */
	void Stop();

	/**
	 * Implemented in derived to use material according to shader program
	 */
	virtual void ProcessMaterial(Model::Material& material) = 0;

protected:
	bool Init(const Tool::CHR* vertexFilePath, const Tool::CHR* fragmentFilePath);
	void Terminate();

	/**
	 * Implemented in derived shader program and called from Init().
	 * 
	 * Attributes are bind to shaders with BindAttribute() calls.
	 */
	virtual void BindShaderAttributes() = 0;
	void BindAttribute(Tool::U32 attributePosition, const Tool::CHR* variableName);

	/**
	 * Implemented in derived shader program and called from Init().
	 * 
	 * Uniform locations can be obtained with GetUniformLocation() calls.
	 */
	virtual void GetShaderUniformLocations() = 0;
	Tool::U32 GetUniformLocation(const Tool::CHR* uniformName);
	
	/**
	 *	Load value to uniform variable in the shader program
	 *	
	 *	@param uniformLocation Location of the uniform variable
	 *	@param value Value to send (can be different types)
	 */
	void LoadUniform(Tool::U32 uniformLocation, Tool::I32 value);
	void LoadUniform(Tool::U32 uniformLocation, Tool::F32 value);
	void LoadUniform(Tool::U32 uniformLocation, bool value);
	void LoadUniform(Tool::U32 uniformLocation, glm::vec2 value);
	void LoadUniform(Tool::U32 uniformLocation, glm::vec3 value);
	void LoadUniform(Tool::U32 uniformLocation, glm::mat4 value);

private:
	bool loadShader(const Tool::CHR* filePath, Tool::U32 shaderType);
	bool linkProgram();

	Tool::U32 _programID;
	Tool::U32 _vertexShaderID;
	Tool::U32 _fragmentShaderID;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__SHADER_PROGRAM_H
