#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


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
	int type;
	vec3 position;
	vec3 color;
	float intensity;
	float range;
	vec3 direction;
	float innerAngle;
	float outerAngle;
} light;

uniform vec3 ambientLight;



vec3 ads(in vec3 position, in vec3 normal) {

	// Ambient
	vec3 ambient = ambientLight;

	// Attenuation
	float attenuation = 1;
	if(light.type != DIRECTIONAL) {
		float distanceSqr = dot(light.position - position, light.position - position);
		float rangeSqr = light.range * light.range;
		attenuation = pow(max(0, 1 - pow((distanceSqr / rangeSqr), 2)), 2);
	}

	// Diffuse
	vec3 lightDirection = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - position);

	float spotIntensity = 1;
	if(light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDirection));
		//if(angle > light.outerAngle) spotIntensity = 0;
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);

	}

	float intensity = max(dot(lightDirection, normal), 0) * spotIntensity;
	vec3 diffuse = material.diffuse * (light.color * intensity);
	

	// Specular
	vec3 specular = vec3(0);
	if(intensity > 0) {
		vec3 reflection = reflect(-lightDirection, normal);
		vec3 viewDirection = normalize(-position);
		intensity = max(dot(reflection, viewDirection), 0) * spotIntensity;
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	// All Light
	return ambient + (diffuse + specular) * attenuation * light.intensity;
}


void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}