#version 410 core

layout(location = 0) in vec2 attrib_position;
layout(location = 1) in vec2 attrib_uvCoord;

uniform mat4 vs_projectionMatrix;
uniform vec4 vs_fs_textColor;

out vec2 varying_uvCoord;

void main()
{
	if(vs_fs_textColor.a == -1)
		gl_Position = vs_projectionMatrix * vec4(attrib_position, -1, 1.0);
	else
		gl_Position = vs_projectionMatrix * vec4(attrib_position, -2, 1.0);
	
	varying_uvCoord = attrib_uvCoord;
}
