#version 410 core

in vec2 varying_uvCoord;

uniform sampler2D fs_diffuseTexture;
uniform vec4 fs_diffuseValue;

out vec4 out_color;

void main()
{
	if(fs_diffuseValue.r == -1)
		out_color = texture(fs_diffuseTexture, varying_uvCoord);
	else
		out_color = fs_diffuseValue;
}
