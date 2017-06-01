#version 410 core

const vec2 invAtan = vec2(0.1591, 0.3183);

in vec3 varying_textureDir;

uniform sampler2D fs_equirectangularMap;

out vec4 out_color;

vec2 GetSphericalDirection(vec3 direction);

void main()
{
	vec2 sphericalDirection = GetSphericalDirection(normalize(varying_textureDir));
	out_color.rgb = texture(fs_equirectangularMap, sphericalDirection).rgb;
	out_color.a = 1.0;
}

///////////////////////////////////////////////////////

vec2 GetSphericalDirection(vec3 direction)
{
	vec2 sphericalDirection = vec2(atan(direction.z, direction.x), asin(direction.y));
	return (sphericalDirection * invAtan) + 0.5;
}
