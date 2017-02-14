#include "AXengine/Gfx/Renderer.h"

#include <GL/glew.h>

namespace AX { namespace Gfx {

void Renderer::Clear(Tool::F32 red, Tool::F32 green, Tool::F32 blue)
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Render(const Model::Mesh& mesh)
{
	glBindVertexArray(mesh.GetVaoID());

	// Enable all the VBOs before rendering
	for( Tool::U32 vbo=0 ; vbo<mesh.GetVboCount() ; vbo++ )
		glEnableVertexAttribArray(vbo);

	glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, nullptr);

	// Disable all the VBOs after rendering
	for( Tool::U32 vbo=0 ; vbo<mesh.GetVboCount() ; vbo++ )
		glDisableVertexAttribArray(vbo);
}
void Renderer::Render(const Model::Mesh& mesh, const Model::Texture& texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());

	Render(mesh);
}

} } // namespace AX::Render
