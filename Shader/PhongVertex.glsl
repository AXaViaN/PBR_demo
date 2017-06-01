#version 410 core

layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_uvCoord;
layout(location = 2) in vec3 attrib_normal;
layout(location = 3) in vec3 attrib_tangent;

uniform mat4 vs_ModelViewProjectionMatrix;
uniform mat4 vs_modelViewMatrix;
uniform mat4 vs_modelMatrix;
uniform mat4 vs_normalMatrix;

out vec3 varying_normal;
out mat3 varying_TBN;
out vec2 varying_uvCoord;
out vec3 varying_onWorldNormal;
out vec3 varying_onWorldPosition;
out vec3 varying_onCameraPosition;

void main()
{
	varying_normal = mat3(vs_normalMatrix) * attrib_normal;
	varying_uvCoord = attrib_uvCoord;
	varying_onWorldNormal = mat3(vs_modelMatrix) * attrib_normal;
	
	// Calculate TangentBitangentNormal Matrix for normal mapping
	vec3 T = normalize(mat3(vs_normalMatrix) * attrib_tangent);
	vec3 B = normalize(cross(T, varying_normal));
	varying_TBN = mat3(T, B, varying_normal);
	
	vec4 position = vec4(attrib_position, 1.0);
	varying_onWorldPosition = vec3(vs_modelMatrix * position);
	varying_onCameraPosition = vec3(vs_modelViewMatrix * position);
	
	gl_Position = vs_ModelViewProjectionMatrix * position;
}
