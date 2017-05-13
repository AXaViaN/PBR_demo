#version 410 core

const float PI = 3.14159265359;

in vec2 varying_uvCoord;

uniform uint fs_sampleCount;

out vec2 out_color;

// Functions
mat3 CalculateTangentWorldMatrix();

float RadicalInverse_VdC(uint bits);
vec2 Hammersley(uint index, uint count);
vec3 ImportanceSampleGGX(uint index, uint count, mat3 TangentWorldMatrix, float roughness);

float GeometrySchlickGGX(float roughness, float cosTheta);
float GeometrySmith(vec3 normal, vec3 cameraRay, vec3 lightRay, float roughness);

// Program entry point
void main()
{
	float cosNormalCameraTheta = varying_uvCoord.x;
	float roughness = varying_uvCoord.y;
	
	mat3 TangentWorldMatrix = CalculateTangentWorldMatrix();
	
	// Assumptions
	vec3 normal = vec3(0.0, 0.0, 1.0);
	vec3 cameraRay = vec3(sqrt(1.0-pow(cosNormalCameraTheta, 2)), 0.0, cosNormalCameraTheta);
	
	// Convolution
	float A = 0.0;
	float B = 0.0;
	for( uint i=0 ; i<fs_sampleCount ; i++ )
	{
		vec3 halfway = ImportanceSampleGGX(i, fs_sampleCount, TangentWorldMatrix, roughness);
		vec3 lightRay = normalize((2.0 * dot(cameraRay, halfway) * halfway) - cameraRay);
		
		float cosNormalLightTheta = max(0.0, lightRay.z);
		if(cosNormalLightTheta > 0.0)
		{
			float cosNormalHalfwayTheta = max(0.0, halfway.z);
			float cosCameraHalfwayTheta = max(0.0, dot(cameraRay, halfway));
			
			float geometry = GeometrySmith(normal, cameraRay, lightRay, roughness);
			float geometryVis = (geometry*cosCameraHalfwayTheta) / (cosNormalHalfwayTheta*cosNormalCameraTheta);
			float Fc = pow(1.0-cosCameraHalfwayTheta, 5);
			
			A += (1.0-Fc) * geometryVis;
			B += Fc * geometryVis;
		}
	}
	A /= float(fs_sampleCount);
	B /= float(fs_sampleCount);
	
	out_color = vec2(A, B);
}

mat3 CalculateTangentWorldMatrix()
{
	vec3 forward = vec3(0.0, 0.0, 1.0);
	
	vec3 up = abs(forward.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 right = normalize(cross(up, forward));
	up = cross(forward, right);
	
	return mat3(right, up, forward);
}

float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
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

float GeometrySchlickGGX(float roughness, float cosTheta)
{
	return cosTheta / (cosTheta * (1.0-roughness) + roughness);
}
float GeometrySmith(vec3 normal, vec3 cameraRay, vec3 lightRay, float roughness)
{
	// Remap roughness for IBL lighting
	float kIBL = pow(roughness, 2) / 2.0;
	
	float cosNormalCameraTheta = max(0.0, dot(normal, cameraRay));
	float cosNormalLightTheta = max(0.0, dot(normal, lightRay));
	
	float cameraGGX = GeometrySchlickGGX(kIBL, cosNormalCameraTheta);
	float lightGGX = GeometrySchlickGGX(kIBL, cosNormalLightTheta);
	
	return cameraGGX * lightGGX;
}
