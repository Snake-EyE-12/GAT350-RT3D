#version 430

in layout(location = 0) vec3 color;

out layout(location = 0) vec4 ocolor;

//uniform float time;

void main()
{
	//vec3 ncolor = color * sin(time * 4) + 1 * 0.5;
	//ocolor = vec4(ncolor, 1);
	ocolor = vec4(color, 1);
}