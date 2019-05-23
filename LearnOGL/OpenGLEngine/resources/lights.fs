#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 camera_pos;

void main()
{
	// Ambient
	float ambient_factor = 0.1;
	vec3 ambient = ambient_factor * light_color;
	
	// Diffuse
	vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light_pos - FragPos);
    float diff_factor = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff_factor * light_color;
	
	// Specular
	vec3 camera_dir = normalize(camera_pos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec_factor = pow(max(dot(camera_dir, reflect_dir), 0.0), 32);
	vec3 specular = spec_factor * light_color;
	
	vec3 combination = (ambient + diffuse + specular) * texture(texture0, TexCoord).rgb;
    FragColor = vec4(combination, 1.0);
}
