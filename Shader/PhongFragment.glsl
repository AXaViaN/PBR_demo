#version 410 core

struct TextureMap {
	sampler2D texture;
	vec3 value;
};
struct Material {
	TextureMap diffuseMap;
	TextureMap specularMap;
	TextureMap emissionMap;
};

struct Light {
	vec3 position; // Position in view space
	
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

in vec3 varying_normal;
in vec2 varying_uvCoord;
in vec3 varying_onCameraPosition;
in vec3 varying_fragmentToLightRay;

uniform Material fs_material;

uniform Light vs_fs_light;

out vec4 out_color;

void getColorFromTextureMaps(inout vec3 diffuseColor, inout vec3 specularColor, inout vec3 emissionColor);
vec3 applyDiffuseLighting(vec3 diffuseColor, vec3 normal, vec3 lightRay);
vec3 applySpecularLighting(vec3 specularColor, vec3 normal, vec3 lightRay);
vec3 applyAmbientLighting(vec3 diffuseColor);

void main()
{
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
	getColorFromTextureMaps(diffuseColor, specularColor, emissionColor);
	
	// Lighting
	vec3 diffuseLight = diffuseColor;
	vec3 specularLight = vec3(0.0);
	vec3 ambientLight = vec3(0.0);
	if(vs_fs_light.diffuse.r != -1)
	{
		vec3 normal = normalize(varying_normal);
		vec3 lightRay = normalize(varying_fragmentToLightRay);
		
		diffuseLight = applyDiffuseLighting(diffuseColor, normal, lightRay);
		specularLight = applySpecularLighting(specularColor, normal, lightRay);
		ambientLight = applyAmbientLighting(diffuseColor);
	}
	
	out_color = vec4(diffuseLight + specularLight + ambientLight, 1.0) + emissionColor;
}

///////////////////////////////////////////////////////

void getColorFromTextureMaps(inout vec3 diffuseColor, inout vec3 specularColor, inout vec3 emissionColor)
{
	if(fs_material.diffuseMap.value.r == -1)
		diffuseColor = texture(fs_material.diffuseMap.texture, varying_uvCoord).rgb;
	else
		diffuseColor = fs_material.diffuseMap.value;
	
	if(fs_material.specularMap.value.r == -1)
		specularColor = texture(fs_material.specularMap.texture, varying_uvCoord).rgb;
	else
		specularColor = fs_material.specularMap.value;
	
	if(fs_material.emissionMap.value.r == -1)
		emissionColor = texture(fs_material.emissionMap.texture, varying_uvCoord).rgb;
	else
		emissionColor = fs_material.emissionMap.value;
}

vec3 applyDiffuseLighting(vec3 diffuseColor, vec3 normal, vec3 lightRay)
{
	float diffuseFactor = max(0.0, dot(normal, lightRay));
	
	return vs_fs_light.diffuse * (diffuseColor * diffuseFactor);
}
vec3 applySpecularLighting(vec3 specularColor, vec3 normal, vec3 lightRay)
{
	vec3 cameraRay = normalize(-varying_onCameraPosition);
	vec3 reflectionDirection = reflect(-lightRay, normal);
	float shininess = 128 * (specularColor.r + specularColor.g + specularColor.b) + 1;
	float specularFactor = pow(max(0.0, dot(cameraRay, reflectionDirection)), shininess);
	
	return vs_fs_light.specular * (specularColor * specularFactor);
}
vec3 applyAmbientLighting(vec3 diffuseColor)
{
	return vs_fs_light.ambient * diffuseColor;
}
