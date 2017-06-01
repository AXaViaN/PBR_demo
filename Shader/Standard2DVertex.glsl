#version 410 core

layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_uvCoord;

uniform mat4 vs_modelMatrix;

out vec2 varying_uvCoord;

void main()
{
	gl_Position = vs_modelMatrix * vec4(attrib_position, 1.0);
	varying_uvCoord = attrib_uvCoord;
}
