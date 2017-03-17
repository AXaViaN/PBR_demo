#version 410 core

in vec2 varying_uvCoord;

uniform sampler2D fs_diffuseTexture;

uniform float fs_gamma;

out vec4 out_color;

void main()
{
	out_color.rgb = pow(texture(fs_diffuseTexture, varying_uvCoord).rgb, vec3(1.0/fs_gamma));
	out_color.a = 1.0;
}
