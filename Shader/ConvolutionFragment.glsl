#version 410 core

const float PI = 3.14159265359;

in vec3 varying_textureDir;

uniform samplerCube fs_cubeMap;

uniform float fs_sampleDelta;

out vec4 out_color;

mat3 CalculateTangentWorldMatrix();

void main()
{
	mat3 TangentWorldMatrix = CalculateTangentWorldMatrix();
	
	// Convolution
	vec3 irradiance = vec3(0.0);
	
	int sampleCount = 0;
	for( float phi=0.0 ; phi<2.0*PI ; phi+=fs_sampleDelta )
	{
		for( float theta=0.0 ; theta<0.5*PI ; theta+=fs_sampleDelta )
		{
			// Spherical to tangent space
			vec3 tangentSample = vec3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
			// Tangent to world space
			vec3 sampleDirection = TangentWorldMatrix * tangentSample;
			
			// cos(theta): light is weaker at larger angles
			// sin(theta): higher hemisphere areas has smaller sample areas
			irradiance += texture(fs_cubeMap, sampleDirection).rgb * cos(theta)*sin(theta);
			sampleCount++;
		}
	}
	irradiance *= PI / float(sampleCount);
	
	out_color = vec4(irradiance, 1.0);
}

mat3 CalculateTangentWorldMatrix()
{
	vec3 forward = normalize(varying_textureDir);
	
	vec3 up = vec3(0, 1, 0);
	vec3 right = cross(up, forward);
	up = cross(forward, right);
	
	return mat3(right, up, forward);
}
