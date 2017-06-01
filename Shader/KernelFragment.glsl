#version 410 core

in vec2 varying_uvCoord;

uniform sampler2D fs_diffuseTexture;

uniform float fs_offset;
uniform float fs_kernel[9];

out vec4 out_color;

void main()
{
	vec2 uvOffset[9];
	uvOffset[0] = vec2(-fs_offset,  fs_offset);
	uvOffset[1] = vec2( 0.0,        fs_offset);
	uvOffset[2] = vec2( fs_offset,  fs_offset);
	uvOffset[3] = vec2(-fs_offset,  0.0);
	uvOffset[4] = vec2( 0.0,        0.0);
	uvOffset[5] = vec2( fs_offset,  0.0);
	uvOffset[6] = vec2(-fs_offset, -fs_offset);
	uvOffset[7] = vec2( 0.0,       -fs_offset);
	uvOffset[8] = vec2( fs_offset, -fs_offset);
	
	out_color = vec4(0.0);
	for( int i=0 ; i<9 ; i++ )
	{
		vec2 uvCoord = varying_uvCoord + uvOffset[i];
		if(uvCoord.x < 0.0 || uvCoord.x > 1.0)
			uvCoord.x = varying_uvCoord.x;
		if(uvCoord.y < 0.0 || uvCoord.y > 1.0)
			uvCoord.y = varying_uvCoord.y;
		
		out_color += texture(fs_diffuseTexture, uvCoord) * fs_kernel[i];
	}
}
