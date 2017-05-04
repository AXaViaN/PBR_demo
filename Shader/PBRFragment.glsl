#version 410 core

#define POINT_LIGHT_COUNT 8
#define SPOT_LIGHT_COUNT 8

const float PI = 3.14159265359;

struct TextureMapVec1 {
	sampler2D texture;
	float value;
};
struct TextureMapVec3 {
	sampler2D texture;
	vec3 value;
};
struct TextureMapVec4 {
	sampler2D texture;
	vec4 value;
};
struct Material {
	TextureMapVec4 albedoMap;
	TextureMapVec3 normalMap;
	TextureMapVec1 metallicMap;
	TextureMapVec1 roughnessMap;
	TextureMapVec1 aoMap;
};

struct DirectionalLight {
	vec3 direction;
	
	vec3 color;
};
struct PointLight {
	vec3 position;
	
	vec3 color;
	
	float constant;
	float linear;
	float quadric;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	
	vec3 color;
	
	float innerCutoff;
	float outerCutoff;
};

in vec3 varying_normal;
in mat3 varying_TBN;
in vec2 varying_uvCoord;
in vec3 varying_onWorldNormal;
in vec3 varying_onWorldPosition;
in vec3 varying_onCameraPosition;

uniform Material fs_material;

uniform DirectionalLight fs_directionalLight;
uniform PointLight fs_pointLight[POINT_LIGHT_COUNT];
uniform SpotLight fs_spotLight[SPOT_LIGHT_COUNT];

out vec4 out_color;

// Functions
void GetValuesFromTextureMaps(inout vec4 albedoColor, inout vec3 normalValue, inout float metallicValue, inout float roughnessValue, inout float aoValue);

float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness);
float GeometrySchlickGGX(float roughness, float cosTheta);
float GeometrySmith(vec3 normal, vec3 cameraRay, vec3 lightRay, float roughness);
vec3 FresnelSchlick(vec3 F0, float cosCameraHalfwayTheta);

vec3 CookTorranceBRDF(vec3 normal, vec3 cameraRay, vec3 lightRay, vec3 halfwayRay, vec3 F0, float roughness, inout vec3 kS);
vec3 CalculateRadiance(vec3 fCookTorrance, vec3 radiantFlux, vec3 normal, vec3 lightRay, vec3 albedo, vec3 kS, float metallic);

vec3 PointLightRadiance(PointLight light, vec3 normal, vec3 cameraRay, vec3 albedo, vec3 F0, float metallic, float roughness);
vec3 DirectionalLightRadiance(DirectionalLight light, vec3 normal, vec3 cameraRay, vec3 albedo, vec3 F0, float metallic, float roughness);
vec3 SpotLightRadiance(SpotLight light, vec3 normal, vec3 cameraRay, vec3 albedo, vec3 F0, float metallic, float roughness);

// Program entry point
void main()
{
	vec4 albedoColor;
	vec3 normalValue;
	float metallicValue;
	float roughnessValue;
	float aoValue;
	GetValuesFromTextureMaps(albedoColor, normalValue, metallicValue, roughnessValue, aoValue);
	
	vec3 normal = normalize(varying_normal);
	if(normalValue.r >= 0)
		normal = normalize(varying_TBN * (normalValue*2.0 - 1.0));
	
	vec3 cameraRay = normalize(-varying_onCameraPosition);
	
	// F0: Base reflectivity
	vec3 F0 = mix(vec3(0.04), albedoColor.rgb, metallicValue);
	vec3 irradiance = vec3(0.0);
	
	// Lighting
	if(fs_directionalLight.color.r != -1)
		irradiance += DirectionalLightRadiance(fs_directionalLight, normal, cameraRay, albedoColor.rgb, F0, metallicValue, roughnessValue);
	
	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		if(fs_pointLight[i].color.r != -1)
			irradiance += PointLightRadiance(fs_pointLight[i], normal, cameraRay, albedoColor.rgb, F0, metallicValue, roughnessValue);
	}
	
	for( int i=0 ; i<SPOT_LIGHT_COUNT ; i++ )
	{
		if(fs_spotLight[i].color.r != -1)
			irradiance += SpotLightRadiance(fs_spotLight[i], normal, cameraRay, albedoColor.rgb, F0, metallicValue, roughnessValue);
	}
	
	// Final color
	vec3 ambient = vec3(0.03) * albedoColor.rgb * aoValue;
	out_color = vec4(irradiance + ambient, albedoColor.a);
}

///////////////////////////////////////////////////////

void GetValuesFromTextureMaps(inout vec4 albedoColor, inout vec3 normalValue, inout float metallicValue, inout float roughnessValue, inout float aoValue)
{
	if(fs_material.albedoMap.value.r == -1)
		albedoColor = texture(fs_material.albedoMap.texture, varying_uvCoord);
	else
		albedoColor = fs_material.albedoMap.value;
	
	if(fs_material.normalMap.value.r == -1)
		normalValue = texture(fs_material.normalMap.texture, varying_uvCoord).rgb;
	else
		normalValue = fs_material.normalMap.value;
	
	if(fs_material.metallicMap.value == -1)
		metallicValue = texture(fs_material.metallicMap.texture, varying_uvCoord).r;
	else
		metallicValue = fs_material.metallicMap.value;
	
	if(fs_material.roughnessMap.value == -1)
		roughnessValue = texture(fs_material.roughnessMap.texture, varying_uvCoord).r;
	else
		roughnessValue = fs_material.roughnessMap.value;
	
	if(fs_material.aoMap.value == -1)
		aoValue = texture(fs_material.aoMap.texture, varying_uvCoord).r;
	else
		aoValue = fs_material.aoMap.value;
}

// PBR functions
float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness)
{
	float a = pow(roughness, 2);
	float a2 = pow(a, 2);
	
	float cosNormalHalfwayTheta = max(0.0, dot(normal, halfway));
	
	float denom = pow(cosNormalHalfwayTheta, 2) * (a2-1.0) + 1.0;
	denom = PI * pow(denom, 2);
	
	return a2 / denom;
}
float GeometrySchlickGGX(float roughness, float cosTheta)
{
	return cosTheta / (cosTheta * (1.0-roughness) + roughness);
}
float GeometrySmith(vec3 normal, vec3 cameraRay, vec3 lightRay, float roughness)
{
	// Remap roughness for direct lighting
	float kDirect = pow(roughness+1.0, 2) / 8.0;
	
	float cosNormalCameraTheta = max(0.0, dot(normal, cameraRay));
	float cosNormalLightTheta = max(0.0, dot(normal, lightRay));
	
	float cameraGGX = GeometrySchlickGGX(kDirect, cosNormalCameraTheta);
	float lightGGX = GeometrySchlickGGX(kDirect, cosNormalLightTheta);
	
	return cameraGGX * lightGGX;
}
vec3 FresnelSchlick(vec3 F0, float cosCameraHalfwayTheta)
{
	return F0 + (1.0-F0)*pow(1.0-cosCameraHalfwayTheta, 5);
}

vec3 CookTorranceBRDF(vec3 normal, vec3 cameraRay, vec3 lightRay, vec3 halfwayRay, vec3 F0, float roughness, inout vec3 kS)
{
	float normalDistribution = NormalDistributionGGX(normal, halfwayRay, roughness);
	float geometry = GeometrySmith(normal, cameraRay, lightRay, roughness);
	vec3 fresnel = FresnelSchlick(F0, max(0.0, dot(cameraRay, halfwayRay)));
	kS = fresnel;
	
	vec3 nominator = normalDistribution * geometry * fresnel;
	float denominator = 4 * max(0.0, dot(normal, cameraRay)) * max(0.0, dot(normal, lightRay));
	return nominator / (denominator+0.001);
}
vec3 CalculateRadiance(vec3 fCookTorrance, vec3 radiantFlux, vec3 normal, vec3 lightRay, vec3 albedo, vec3 kS, float metallic)
{
	// Reflectance
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;
	
	vec3 fLambert = albedo / PI;
	float cosNormalLightTheta = max(0.0, dot(normal, lightRay));
	return (kD*fLambert + fCookTorrance) * radiantFlux * cosNormalLightTheta;
}

// Light functions
vec3 PointLightRadiance(PointLight light, vec3 normal, vec3 cameraRay, vec3 albedo, vec3 F0, float metallic, float roughness)
{
	// Light radiant flux
	vec3 lightRay = normalize(light.position - varying_onCameraPosition);
	vec3 halfwayRay = normalize(cameraRay + lightRay);
	
	float lightDistance = length(light.position - varying_onCameraPosition);
	float attenuation = 1.0 / (light.constant + light.linear*lightDistance + light.quadric*pow(lightDistance, 2));
	vec3 radiantFlux = light.color * attenuation;
	
	// BRDF
	vec3 kS;
	vec3 fCookTorrance = CookTorranceBRDF(normal, cameraRay, lightRay, halfwayRay, F0, roughness, kS);
	
	return CalculateRadiance(fCookTorrance, radiantFlux, normal, lightRay, albedo, kS, metallic);
}
vec3 DirectionalLightRadiance(DirectionalLight light, vec3 normal, vec3 cameraRay, vec3 albedo, vec3 F0, float metallic, float roughness)
{
	// Light radiant flux
	vec3 lightRay = normalize(-light.direction);
	vec3 halfwayRay = normalize(cameraRay + lightRay);
	vec3 radiantFlux = light.color;
	
	// BRDF
	vec3 kS;
	vec3 fCookTorrance = CookTorranceBRDF(normal, cameraRay, lightRay, halfwayRay, F0, roughness, kS);
	
	return CalculateRadiance(fCookTorrance, radiantFlux, normal, lightRay, albedo, kS, metallic);
}
vec3 SpotLightRadiance(SpotLight light, vec3 normal, vec3 cameraRay, vec3 albedo, vec3 F0, float metallic, float roughness)
{
	// Light radiant flux
	vec3 lightRay = normalize(light.position - varying_onCameraPosition);
	vec3 halfwayRay = normalize(cameraRay + lightRay);
	vec3 spotRay = normalize(-light.direction);
	float theta = dot(lightRay, spotRay);
	vec3 radiantFlux = vec3(0.0);
	if(theta > light.outerCutoff)
	{
		radiantFlux = light.color;
		
		if(theta < light.innerCutoff)
		{
			float attenuation = (theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff);
			radiantFlux *= attenuation;
		}
	}
	
	// BRDF
	vec3 kS;
	vec3 fCookTorrance = CookTorranceBRDF(normal, cameraRay, lightRay, halfwayRay, F0, roughness, kS);
	
	return CalculateRadiance(fCookTorrance, radiantFlux, normal, lightRay, albedo, kS, metallic);
}
