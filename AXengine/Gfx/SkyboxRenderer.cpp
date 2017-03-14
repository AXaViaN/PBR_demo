#include "AXengine/Gfx/SkyboxRenderer.h"

#include "AXengine/Shader/ShaderProgram.h"
#include "AXengine/Gfx/Renderer.h"
#include <GL/glew.h>

namespace AX { namespace Gfx {

void SkyboxRenderer::Render(Entity::Cubemap& skybox)
{
	glDisable(GL_DEPTH_TEST);
	skybox.material.shader->Start();

	Renderer::PrepareShader(skybox.material.shader);
	skybox.material.shader->ProcessMaterial(skybox.material);
	
	glBindVertexArray(skybox.mesh.GetVaoID());
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);

	skybox.material.shader->Stop();
	glEnable(GL_DEPTH_TEST);
}

} } // namespace AX::Gfx
