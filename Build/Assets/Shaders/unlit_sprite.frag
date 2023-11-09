#version 430

in layout(location = 0) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

uniform vec4 color;
uniform vec2 offset;
uniform vec2 tiling;

layout(binding = 0) uniform sampler2D tex;

uniform struct Material
{
	vec3 albedo;
	vec2 offset;
	vec2 tiling;
} material;


void main()
{
	vec4 texcolor = texture(tex, ftexcoord);

	if(texcolor.a < 0.5) discard;

	ocolor = texcolor * vec4(material.albedo, 1);
}