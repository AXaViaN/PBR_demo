#version 410 core

in vec2 varying_uvCoord;

uniform sampler2D fs_diffuseTexture;

uniform float fs_gamma;
uniform float fs_hdrExposure;

out vec4 out_color;

void main()
{
	vec3 sceneColor = texture(fs_diffuseTexture, varying_uvCoord).rgb;
	
	// HDR tone mapping
	sceneColor = vec3(1.0) - exp(-sceneColor * fs_hdrExposure);
	// Gamma correction
	sceneColor = pow(sceneColor, vec3(1.0/fs_gamma));
	
	out_color = vec4(sceneColor, 1.0);
}
