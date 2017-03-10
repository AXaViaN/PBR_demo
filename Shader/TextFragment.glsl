#version 410 core

in vec2 varying_uvCoord;

uniform sampler2D fs_character;
uniform vec4 vs_fs_textColor;

out vec4 out_color;

void main()
{
	if(vs_fs_textColor.a == -1)
		out_color = vec4(vs_fs_textColor.rgb, texture(fs_character, varying_uvCoord).r);
	else
		out_color = vs_fs_textColor;
}
