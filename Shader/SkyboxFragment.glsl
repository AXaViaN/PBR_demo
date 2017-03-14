#version 410 core

in vec3 varying_textureDir;

uniform samplerCube fs_cubeMap;

out vec4 out_color;

void main()
{
	out_color = texture(fs_cubeMap, varying_textureDir);
}
