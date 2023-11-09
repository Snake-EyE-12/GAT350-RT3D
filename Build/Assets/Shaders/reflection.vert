#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	// transform vertex position and normal to world space; view independent
	vec3 position = vec3(model * vec4(vposition, 1));
	vec3 normal = normalize(mat3(model) * vnormal);


	// inverse = viewSpace -> worldSpace
	vec3 viewPosition = inverse(view)[3].xyz;

	vec3 viewDirection = normalize(position - viewPosition);

	// reflect about normal
	otexcoord = reflect(viewDirection, normal);

	mat4 vp = projection * view * model;
	gl_Position = vp * vec4(vposition, 1.0);
}