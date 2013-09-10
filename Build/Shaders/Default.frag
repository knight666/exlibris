#version 330 core

uniform vec4 uniColor;

out vec4 fragColor;

vec4 Lighting(vec3 a_Normal, vec3 a_LightPos, vec3 a_Eye, vec3 a_Attenuation, float a_LightIntensity, float a_Shininess)
{
	vec3 N = normalize(a_Normal);
	vec3 L = normalize(a_LightPos - a_Eye);
	vec3 E = normalize(-a_Eye);
	vec3 R = normalize(-reflect(L, N));

	float diffuse = max(dot(N, L), 0.0);
	
	float specular = pow(max(dot(R, E), 0.0), a_Shininess);
	
	float attenuation = a_LightIntensity;
	
	return (vec4(1.0, 1.0, 1.0, 1.0) * diffuse) + (vec4(1.0, 1.0, 1.0, 1.0) * specular * attenuation);
}

void main()
{
	fragColor = uniColor;
}