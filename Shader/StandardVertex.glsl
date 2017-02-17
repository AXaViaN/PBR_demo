#version 410 core

layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_uvCoord;

uniform mat4 vs_ModelViewProjectionMatrix;

out vec2 uvCoord;

void main()
{
	gl_Position = vs_ModelViewProjectionMatrix * vec4(attrib_position, 1.0);
	uvCoord = attrib_uvCoord;
}
