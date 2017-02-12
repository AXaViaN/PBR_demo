/**
 *	File: AXengine/Shader/ShaderProgram.h
 *	Purpose: Base class for shaders
 */

#ifndef __AX__SHADER__SHADER_PROGRAM_H
#define __AX__SHADER__SHADER_PROGRAM_H

#include "AXengine/Tool/Utility.h"

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

private:
	bool loadShader(const Tool::CHR* filePath, Tool::U32 shaderType);
	bool linkProgram();

	Tool::U32 _programID;
	Tool::U32 _vertexShaderID;
	Tool::U32 _fragmentShaderID;

};

} } // namespace AX::Shader

#endif // __AX__SHADER__SHADER_PROGRAM_H
