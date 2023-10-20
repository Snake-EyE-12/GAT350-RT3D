#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
out layout(location = 3) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 offset;
	vec2 tiling;
} material;


uniform struct Light
{
	vec3 position;
	vec3 dcolor;
} light;

uniform vec3 ambientLight;

vec3 ads(in vec3 position, in vec3 normal) {

	// Ambient
	vec3 ambient = ambientLight;

	// Diffuse
	vec3 lightDirection = normalize(light.position - position);
	float intensity = max(dot(lightDirection, normal), 0);
	vec3 diffuse = material.diffuse * (light.dcolor * intensity);
	
	// Specular
	vec3 specular = vec3(0);
	if(intensity > 0) {
		vec3 reflection = reflect(-lightDirection, normal);
		vec3 viewDirection = normalize(-position);
		intensity = max(dot(reflection, viewDirection), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	// All Light
	return ambient + diffuse + specular;
}

void main()
{
	mat4 modelView = view * model;

	// conversion to world view
	oposition = vec3(modelView * vec4(vposition, 1));
	onormal = normalize(mat3(modelView) * vnormal);
	otexcoord = vtexcoord * material.tiling + material.offset;


	ocolor = vec4(ads(oposition, onormal), 1);

	mat4 mvp = projection * view * model;

	gl_Position = mvp * vec4(vposition, 1.0);


}