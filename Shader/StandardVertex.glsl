#version 410 core

in vec3 vs_position;
in vec2 vs_uvCoord;

out vec2 fs_uvCoord;

void main()
{
	gl_Position = vec4(vs_position, 1.0);
	fs_uvCoord = vs_uvCoord;
}
