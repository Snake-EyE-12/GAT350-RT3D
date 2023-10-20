#version 430

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;


layout(binding = 0) uniform sampler2D tex;




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
	ocolor = vec4(ads(fposition, fnormal), 1);
	vec4 texcolor = texture(tex, ftexcoord);
}