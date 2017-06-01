#version 410 core

layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_uvCoord;

out vec2 varying_uvCoord;

void main()
{
	gl_Position = vec4(attrib_position, 1.0);
	varying_uvCoord = attrib_uvCoord;
}
