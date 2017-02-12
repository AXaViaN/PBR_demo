#version 410 core

in vec3 vs_position;

void main()
{
	gl_Position = vec4(vs_position, 1.0);
}
