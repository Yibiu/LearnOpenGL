#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};

uniform sampler2D texture0;
uniform vec3 camera_pos;
uniform material obj;
uniform light lamp;

void main()
{
	// Ambient
	vec3 ambient = lamp.ambient * obj.ambient;
	
	// Diffuse
	vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(lamp.position - FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * obj.diffuse);
	
	// Specular
	vec3 camera_dir = normalize(camera_pos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(camera_dir, reflect_dir), 0.0), obj.shininess);
	vec3 specular = lamp.specular * (spec * obj.specular);
	
	// Attenuation
	float distance = length(lamp.position - FragPos);
	float attenuation = 1.0 / (lamp.constant + lamp.linear * distance + lamp.quadratic * (distance * distance));
	
	vec3 combination = attenuation * (ambient + diffuse + specular) * texture(texture0, TexCoord).rgb;
    FragColor = vec4(combination, 1.0);
}
