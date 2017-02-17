#version 410 core

in vec2 uvCoord;

uniform sampler2D fs_diffuseTexture;
uniform vec3 fs_diffuseValue;

out vec4 out_color;

void main()
{
	if(fs_diffuseValue.r == -1)
		out_color = texture(fs_diffuseTexture, uvCoord);
	else
		out_color = vec4(fs_diffuseValue, 1.0);
}
