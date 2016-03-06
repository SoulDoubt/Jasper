#version 330

out vec4 fcolor;
smooth in vec2 outTexCoords;
smooth in vec3 outNormal;
smooth in vec3 eyeSpaceVertPosition;
smooth in vec4 outVertColor;

uniform sampler2D colorMap;
uniform int isTextured;
uniform vec3 cameraPosition;

struct d_light{
	vec3 Color;
	vec3 Position;
	float AmbientIntensity;
	float DiffuseIntensity;
	vec3 Direction;
	
	float ConstAtten;
	float LinearAtten;
	float Exp;
};

struct material{
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float ns;
};

uniform material material0;

uniform d_light light0;

void main()
{	
	vec3 light_direction = normalize(light0.Position - eyeSpaceVertPosition);
	vec3 norm = normalize(outNormal);	
	
	vec3 ambient_color = light0.Color * material0.ka * light0.AmbientIntensity;

	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse_color = light0.Color * light0.DiffuseIntensity * (diff * material0.kd);

	vec3 view_direction = normalize(cameraPosition - eyeSpaceVertPosition);
	vec3 reflection_direction = reflect(-light_direction, norm);
	float reflectionComponent = pow(max(dot(view_direction, reflection_direction), 0.0), material0.ns);
	vec3 specular_color = light0.Color * (reflectionComponent * material0.ks);
		
	vec4 texture_color = texture(colorMap, outTexCoords);
	vec3 lighting_contribution = ambient_color + diffuse_color + specular_color;

	fcolor =  texture_color * vec4(lighting_contribution, 1.0);				

}