#version 410 core

#define POINT_LIGHT_COUNT 8
#define SPOT_LIGHT_COUNT 8

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
	TextureMapVec4 diffuseMap;
	TextureMapVec3 specularMap;
	TextureMapVec3 emissionMap;
	TextureMapVec1 reflectionMap;
	
	float shininess;
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
in vec3 varying_onWorldNormal;
in vec3 varying_onWorldPosition;
in vec3 varying_onCameraPosition;

uniform float fs_nearPlane;
uniform float fs_farPlane;

uniform vec3 fs_cameraPosition;

uniform Material fs_material;
uniform samplerCube fs_environmentMap;

uniform DirectionalLight fs_directionalLight;
uniform PointLight fs_pointLight[POINT_LIGHT_COUNT];
uniform SpotLight fs_spotLight[SPOT_LIGHT_COUNT];

out vec4 out_color;

// Functions
float getFragDepth();
void getValuesFromTextureMaps(inout vec4 diffuseColor, inout vec3 specularColor, inout vec3 emissionColor, inout float reflectionFactor);

vec3 applyDiffuseLighting(vec3 lightColor, vec3 diffuseColor, vec3 normal, vec3 lightRay);
vec3 applySpecularLighting(vec3 lightColor, vec3 specularColor, vec3 normal, vec3 lightRay);
vec3 applyAmbientLighting(vec3 lightColor, vec3 diffuseColor);

Color calculatePointLight(PointLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal);
Color calculateDirectionalLight(DirectionalLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal);
Color calculateSpotLight(SpotLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal);

vec3 calculateEnvironmentReflection();

// Program entry point
void main()
{
	vec4 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
	float reflectionFactor;
	getValuesFromTextureMaps(diffuseColor, specularColor, emissionColor, reflectionFactor);
	
	vec3 normal = normalize(varying_normal);
	
	// Lighting
	Color directionalLight;
	directionalLight.diffuse = vec3(0.0);
	directionalLight.specular = vec3(0.0);
	directionalLight.ambient = vec3(0.0);
	if(fs_directionalLight.color.diffuse.r != -1)
	{
		directionalLight = calculateDirectionalLight(fs_directionalLight, diffuseColor.rgb, specularColor, normal);
	}
	
	Color pointLights;
	pointLights.diffuse = vec3(0.0);
	pointLights.specular = vec3(0.0);
	pointLights.ambient = vec3(0.0);
	for( int i=0 ; i<POINT_LIGHT_COUNT ; i++ )
	{
		if(fs_pointLight[i].color.diffuse.r != -1)
		{
			Color result = calculatePointLight(fs_pointLight[i], diffuseColor.rgb, specularColor, normal);
			
			pointLights.diffuse += result.diffuse;
			pointLights.specular += result.specular;
			pointLights.ambient += result.ambient;
		}
	}
	
	Color spotLights;
	spotLights.diffuse = vec3(0.0);
	spotLights.specular = vec3(0.0);
	spotLights.ambient = vec3(0.0);
	for( int i=0 ; i<SPOT_LIGHT_COUNT ; i++ )
	{
		if(fs_spotLight[i].color.diffuse.r != -1)
		{
			Color result = calculateSpotLight(fs_spotLight[i], diffuseColor.rgb, specularColor, normal);
			
			spotLights.diffuse += result.diffuse;
			spotLights.specular += result.specular;
			spotLights.ambient += result.ambient;
		}
	}
	
	vec3 totalDiffuse = directionalLight.diffuse + pointLights.diffuse + spotLights.diffuse;
	vec3 totalSpecular = directionalLight.specular + pointLights.specular + spotLights.specular;
	vec3 totalAmbient = directionalLight.ambient + pointLights.ambient + spotLights.ambient;
	
	// Mix colors with environment
	vec3 totalColor = totalDiffuse + totalAmbient;
	if(reflectionFactor > 0)
		totalColor = mix(totalColor, calculateEnvironmentReflection(), reflectionFactor);
	
	totalColor += totalSpecular + emissionColor;
	out_color = vec4(totalColor, diffuseColor.a);
}

///////////////////////////////////////////////////////

float getFragDepth()
{
	return (fs_nearPlane * fs_farPlane) / (fs_farPlane + gl_FragCoord.z * (fs_nearPlane - fs_farPlane));
}
void getValuesFromTextureMaps(inout vec4 diffuseColor, inout vec3 specularColor, inout vec3 emissionColor, inout float reflectionFactor)
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
	
	if(fs_material.reflectionMap.value == -1)
		reflectionFactor = texture(fs_material.reflectionMap.texture, varying_uvCoord).r;
	else
		reflectionFactor = fs_material.reflectionMap.value;
}

vec3 applyDiffuseLighting(vec3 lightColor, vec3 diffuseColor, vec3 normal, vec3 lightRay)
{
	float diffuseFactor = max(0.0, dot(normal, lightRay));
	
	return lightColor * (diffuseColor * diffuseFactor);
}
vec3 applySpecularLighting(vec3 lightColor, vec3 specularColor, vec3 normal, vec3 lightRay)
{
	vec3 cameraRay = normalize(-varying_onCameraPosition);
	vec3 halfwayDirection = normalize(cameraRay + lightRay);
	float specularFactor = pow(max(0.0, dot(normal, halfwayDirection)), fs_material.shininess);
	
	return lightColor * (specularColor * specularFactor);
}
vec3 applyAmbientLighting(vec3 lightColor, vec3 diffuseColor)
{
	return lightColor * diffuseColor;
}

Color calculatePointLight(PointLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal)
{
	Color pointLight;
	
	vec3 lightRay = normalize(light.position - varying_onCameraPosition);
	
	pointLight.diffuse = applyDiffuseLighting(light.color.diffuse, diffuseColor, normal, lightRay);
	pointLight.specular = applySpecularLighting(light.color.specular, specularColor, normal, lightRay);
	pointLight.ambient = applyAmbientLighting(light.color.ambient, diffuseColor);
	
	float lightDistance = length(light.position - varying_onCameraPosition);
	float attenuation = 1.0 / (light.constant + light.linear*lightDistance + light.quadric*pow(lightDistance, 2));
	
	pointLight.diffuse *= attenuation;
	pointLight.specular *= attenuation;
	pointLight.ambient *= attenuation;
	
	return pointLight;
}
Color calculateDirectionalLight(DirectionalLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal)
{
	Color directionalLight;
	
	vec3 lightRay = normalize(-light.direction);
	
	directionalLight.diffuse = applyDiffuseLighting(light.color.diffuse, diffuseColor, normal, lightRay);
	directionalLight.specular = applySpecularLighting(light.color.specular, specularColor, normal, lightRay);
	directionalLight.ambient = applyAmbientLighting(light.color.ambient, diffuseColor);
	
	return directionalLight;
}
Color calculateSpotLight(SpotLight light, vec3 diffuseColor, vec3 specularColor, vec3 normal)
{
	Color spotLight;
	
	vec3 lightRay = normalize(light.position - varying_onCameraPosition);
	vec3 spotRay = normalize(-light.direction);
	float theta = dot(lightRay, spotRay);
	
	spotLight.diffuse = vec3(0.0);
	spotLight.specular = vec3(0.0);
	spotLight.ambient = applyAmbientLighting(light.color.ambient, diffuseColor);
	float attenuation = 1.0;
	
	if(theta > light.outerCutoff)
	{
		spotLight.diffuse = applyDiffuseLighting(light.color.diffuse, diffuseColor, normal, lightRay);
		spotLight.specular = applySpecularLighting(light.color.specular, specularColor, normal, lightRay);
		
		if(theta < light.innerCutoff)
		{
			attenuation = (theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff);
		}
	}
	else
	{
		attenuation = 0.0;
	}
	
	spotLight.diffuse *= attenuation;
	spotLight.specular *= attenuation;
	spotLight.ambient *= attenuation;
	
	return spotLight;
}

vec3 calculateEnvironmentReflection()
{
	vec3 cameraRay = normalize(varying_onWorldPosition - fs_cameraPosition);
	vec3 reflectionDirection = reflect(cameraRay, normalize(varying_onWorldNormal));
	
	return texture(fs_environmentMap, reflectionDirection).rgb;
}
