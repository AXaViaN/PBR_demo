#version 410 core

in vec3 varying_normal;
in vec2 varying_uvCoord;
in vec3 varying_onCameraPosition;
in vec3 varying_fragmentToLightRay;

uniform sampler2D fs_diffuseTexture;
uniform vec3 fs_diffuseValue;

uniform vec3 fs_lightColor;
float ambientStrength = 0.15;
float specularStrength = 0.5;
float shininess = 32;

out vec4 out_color;

void main()
{
	vec4 diffuseColor;
	if(fs_diffuseValue.r == -1)
		diffuseColor = texture(fs_diffuseTexture, varying_uvCoord);
	else
		diffuseColor = vec4(fs_diffuseValue, 1.0);
	
	// Lighting
	vec3 diffuseLight = vec3(1.0);
	vec3 ambientLight = vec3(0.0);
	vec3 specularLight = vec3(0.0);
	if(fs_lightColor.r != -1)
	{
		// Ambient
		ambientLight = fs_lightColor * ambientStrength;
		
		// Diffuse
		vec3 normal = normalize(varying_normal);
		vec3 lightRay = normalize(varying_fragmentToLightRay);
		float diffuseStrength = max(0.0, dot(normal, lightRay));
		
		diffuseLight = fs_lightColor * diffuseStrength;
		
		// Specular
		vec3 cameraRay = normalize(-varying_onCameraPosition);
		vec3 reflectionDirection = reflect(-lightRay, normal);
		
		float specularFactor = pow(max(0.0, dot(cameraRay, reflectionDirection)), shininess);
		specularLight = fs_lightColor * specularStrength * specularFactor;
	}
	
	out_color = vec4(diffuseLight+ambientLight+specularLight, 1.0) * diffuseColor;
}
