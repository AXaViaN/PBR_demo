#version 410 core

#define POINT_LIGHT_COUNT 8
#define SPOT_LIGHT_COUNT 8

struct TextureMapVec3 {
	sampler2D texture;
	vec3 value;
};
struct TextureMapVec4 {
	sampler2D texture;
	vec4 value;
};
struct Material {
	TextureMapVec4 diffuseMap;
	TextureMapVec3 specularMap;
	TextureMapVec3 emissionMap;
};

struct Color {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};
struct DirectionalLight {
	vec3 direction;
	
	Color color;
};
struct PointLight {
	vec3 position;
	
	Color color;
	
	float constant;
	float linear;
	float quadric;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	
	Color color;
	
	float innerCutoff;
	float outerCutoff;
};

in vec3 varying_normal;
in vec2 varying_uvCoord;
in vec3 varying_onCameraPosition;

uniform float fs_nearPlane;
uniform float fs_farPlane;

uniform Material fs_material;

uniform DirectionalLight fs_directionalLight;
uniform PointLight fs_pointLight[POINT_LIGHT_COUNT];
uniform SpotLight fs_spotLight[SPOT_LIGHT_COUNT];

out vec4 out_color;

// Functions
float getFragDepth();
void getColorFromTextureMaps(inout vec4 diffuseColor, inout vec3 specularColor, inout vec3 emissionColor);

vec3 applyDiffuseLighting(vec3 lightColor, vec3 diffuseColor, vec3 normal, vec3 lightRay);
vec3 applySpecularLighting(vec3 lightColor, vec3 specularColor, vec3 normal, vec3 lightRay);
vec3 applyAmbientLighting(vec3 lightColor, vec3 diffuseColor);

vec3 calculatePointLight(PointLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal);
vec3 calculateSpotLight(SpotLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal);

// Program entry point
void main()
{
	vec4 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
	getColorFromTextureMaps(diffuseColor, specularColor, emissionColor);
	
	if(diffuseColor.a < 0.05)
		discard;
	
	vec3 normal = normalize(varying_normal);
	
	// Lighting
	vec3 directionalLight = vec3(0.0);
	if(fs_directionalLight.color.diffuse.r != -1)
	{
		directionalLight = calculateDirectionalLight(fs_directionalLight, diffuseColor.rgb, specularColor, normal);
	}
	
	vec3 pointLights = vec3(0.0);
	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		if(fs_pointLight[i].color.diffuse.r != -1)
		{
			pointLights += calculatePointLight(fs_pointLight[i], diffuseColor.rgb, specularColor, normal);
		}
	}
	
	vec3 spotLights = vec3(0.0);
	for( int i=0 ; i<SPOT_LIGHT_COUNT ; i++ )
	{
		if(fs_spotLight[i].color.diffuse.r != -1)
		{
			spotLights += calculateSpotLight(fs_spotLight[i], diffuseColor.rgb, specularColor, normal);
		}
	}
	
	out_color = vec4(directionalLight + pointLights + spotLights + emissionColor, diffuseColor.a);
}

///////////////////////////////////////////////////////

float getFragDepth()
{
	return (fs_nearPlane * fs_farPlane) / (fs_farPlane + gl_FragCoord.z * (fs_nearPlane - fs_farPlane));
}
void getColorFromTextureMaps(inout vec4 diffuseColor, inout vec3 specularColor, inout vec3 emissionColor)
{
	if(fs_material.diffuseMap.value.r == -1)
		diffuseColor = texture(fs_material.diffuseMap.texture, varying_uvCoord);
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

vec3 applyDiffuseLighting(vec3 lightColor, vec3 diffuseColor, vec3 normal, vec3 lightRay)
{
	float diffuseFactor = max(0.0, dot(normal, lightRay));
	
	return lightColor * (diffuseColor * diffuseFactor);
}
vec3 applySpecularLighting(vec3 lightColor, vec3 specularColor, vec3 normal, vec3 lightRay)
{
	vec3 cameraRay = normalize(-varying_onCameraPosition);
	vec3 reflectionDirection = reflect(-lightRay, normal);
	float shininess = 128 * (specularColor.r + specularColor.g + specularColor.b) + 1;
	float specularFactor = pow(max(0.0, dot(cameraRay, reflectionDirection)), shininess);
	
	return lightColor * (specularColor * specularFactor);
}
vec3 applyAmbientLighting(vec3 lightColor, vec3 diffuseColor)
{
	return lightColor * diffuseColor;
}

vec3 calculatePointLight(PointLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal)
{
	vec3 lightRay = normalize(light.position - varying_onCameraPosition);
	
	vec3 diffuseLight = applyDiffuseLighting(light.color.diffuse, diffuseColor, normal, lightRay);
	vec3 specularLight = applySpecularLighting(light.color.specular, specularColor, normal, lightRay);
	vec3 ambientLight = applyAmbientLighting(light.color.ambient, diffuseColor);
	
	float lightDistance = length(light.position - varying_onCameraPosition);
	float attenuation = 1.0 / (light.constant + light.linear*lightDistance + light.quadric*pow(lightDistance, 2));
	
	return (diffuseLight + specularLight + ambientLight) * attenuation;
}
vec3 calculateDirectionalLight(DirectionalLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal)
{
	vec3 lightRay = normalize(-light.direction);
	
	vec3 diffuseLight = applyDiffuseLighting(light.color.diffuse, diffuseColor, normal, lightRay);
	vec3 specularLight = applySpecularLighting(light.color.specular, specularColor, normal, lightRay);
	vec3 ambientLight = applyAmbientLighting(light.color.ambient, diffuseColor);
	
	return diffuseLight + specularLight + ambientLight;
}
vec3 calculateSpotLight(SpotLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal)
{
	vec3 lightRay = normalize(light.position - varying_onCameraPosition);
	vec3 spotRay = normalize(-light.direction);
	float theta = dot(lightRay, spotRay);
	
	vec3 diffuseLight = vec3(0.0);
	vec3 specularLight = vec3(0.0);
	vec3 ambientLight = applyAmbientLighting(light.color.ambient, diffuseColor);
	float attenuation = 1.0;
	
	if(theta > light.outerCutoff)
	{
		diffuseLight = applyDiffuseLighting(light.color.diffuse, diffuseColor, normal, lightRay);
		specularLight = applySpecularLighting(light.color.specular, specularColor, normal, lightRay);
		
		if(theta < light.innerCutoff)
		{
			attenuation = (theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff);
		}
	}
	else
	{
		attenuation = 0.0;
	}
	
	return (diffuseLight + specularLight + ambientLight) * attenuation;
}
