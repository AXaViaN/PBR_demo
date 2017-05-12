#version 410 core

const float PI = 3.14159265359;

in vec3 varying_textureDir;

uniform samplerCube fs_cubeMap;

uniform uint fs_sampleCount;
uniform float fs_roughness;
uniform float fs_resolution;

out vec4 out_color;

// Functions
mat3 CalculateTangentWorldMatrix();

float RadicalInverse_VdC(uint bits);
vec2 Hammersley(uint index, uint count);
vec3 ImportanceSampleGGX(uint index, uint count, mat3 TangentWorldMatrix, float roughness);

float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness);

// Program entry point
void main()
{
	mat3 TangentWorldMatrix = CalculateTangentWorldMatrix();
	
	// Assumptions
	vec3 normal = normalize(varying_textureDir);
	vec3 reflectionRay = normal;
	vec3 cameraRay = normal;
	
	// Convolution
	vec3 prefilteredColor = vec3(0.0);
	
	float totalWeight = 0.0;
	for( uint i=0 ; i<fs_sampleCount ; i++ )
	{
		vec3 halfway = ImportanceSampleGGX(i, fs_sampleCount, TangentWorldMatrix, fs_roughness);
		vec3 lightRay = normalize((2.0 * dot(cameraRay, halfway) * halfway) - cameraRay);
		
		float cosNormalLightTheta = max(0.0, dot(normal, lightRay));
		if(cosNormalLightTheta > 0.0)
		{
			// Calculate mipmap level
			float cosNormalHalfwayTheta = max(0.0, dot(normal, halfway));
			float cosHalfwayCameraTheta = max(0.0, dot(halfway, cameraRay));
			float normalDistribution = NormalDistributionGGX(normal, halfway, fs_roughness);
			float probabilityDensity = (normalDistribution * cosNormalHalfwayTheta/(4.0*cosHalfwayCameraTheta)) + 0.0001;
			
			float saTexel = 4.0*PI / (6.0*pow(fs_resolution, 2));
			float saSample = 1.0 / (float(fs_sampleCount)*probabilityDensity + 0.0001);
			float mipmapLevel = fs_roughness==0.0 ? 0.0 : 0.5*log2(saSample/saTexel);
			
			// Sample
			prefilteredColor += textureLod(fs_cubeMap, lightRay, mipmapLevel).rgb * cosNormalLightTheta;
			totalWeight += cosNormalLightTheta;
		}
	}
	prefilteredColor /= totalWeight;
	
	out_color = vec4(prefilteredColor, 1.0);
}

mat3 CalculateTangentWorldMatrix()
{
	vec3 forward = normalize(varying_textureDir);
	
	vec3 up = abs(forward.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 right = normalize(cross(up, forward));
	up = cross(forward, right);
	
	return mat3(right, up, forward);
}

float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 1u) | ((bits & 0xCCCCCCCCu) >> 1u);
	bits = ((bits & 0x0F0F0F0Fu) << 1u) | ((bits & 0xF0F0F0F0u) >> 1u);
	bits = ((bits & 0x00FF00FFu) << 1u) | ((bits & 0xFF00FF00u) >> 1u);
	return float(bits) * 2.3283064365386963e-10; // float(0x100000000u)
}
vec2 Hammersley(uint index, uint count)
{
	return vec2(float(index)/float(count), RadicalInverse_VdC(index));
}
vec3 ImportanceSampleGGX(uint index, uint count, mat3 TangentWorldMatrix, float roughness)
{
	vec2 hammersleySample = Hammersley(index, count);
	float a = pow(roughness, 2);
	
	float phi = 2.0 * PI * hammersleySample.x;
	float cosTheta = sqrt((1.0-hammersleySample.y) / (1.0 + (pow(a,2)-1.0)*hammersleySample.y));
	float sinTheta = sqrt(1.0 - pow(cosTheta, 2));
	
	// Spherical to tangent space
	vec3 tangentSample = vec3(sinTheta*cos(phi), sinTheta*sin(phi), cosTheta);
	// Tangent to world space
	vec3 sampleDirection = TangentWorldMatrix * tangentSample;
	
	return normalize(sampleDirection);
}

float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness)
{
	float a = pow(roughness, 2);
	float a2 = pow(a, 2);
	
	float cosNormalHalfwayTheta = max(0.0, dot(normal, halfway));
	
	float denom = pow(cosNormalHalfwayTheta, 2) * (a2-1.0) + 1.0;
	denom = PI * pow(denom, 2);
	
	return a2 / denom;
}
