#version 410 core

struct Light {
	vec3 position; // Position in view space
	
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_uvCoord;
layout(location = 2) in vec3 attrib_normal;

uniform mat4 vs_ModelViewProjectionMatrix;
uniform mat4 vs_modelViewMatrix;
uniform mat4 vs_normalMatrix;

uniform Light vs_fs_light;

out vec3 varying_normal;
out vec2 varying_uvCoord;
out vec3 varying_onCameraPosition;
out vec3 varying_fragmentToLightRay;

void main()
{
	varying_normal = mat3(vs_normalMatrix) * attrib_normal;
	varying_uvCoord = attrib_uvCoord;
	
	vec4 position = vec4(attrib_position, 1.0);
	varying_onCameraPosition = vec3(vs_modelViewMatrix * position);
	varying_fragmentToLightRay = vs_fs_light.position - varying_onCameraPosition;
	
	gl_Position = vs_ModelViewProjectionMatrix * position;
}
