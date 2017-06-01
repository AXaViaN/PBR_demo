#version 410 core

layout(location = 0) in vec3 attrib_position;

uniform mat4 vs_ViewProjectionMatrix;

out vec3 varying_textureDir;

void main()
{
	gl_Position = vs_ViewProjectionMatrix * vec4(attrib_position, 1.0);
	varying_textureDir = attrib_position;
}
