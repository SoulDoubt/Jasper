#version 330

struct point_light{
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

out vec4 fcolor;

smooth in vec2 v_texCoords;
smooth in vec3 v_normal;
smooth in vec4 v_vertColor;
smooth in vec3 v_fragPosition;
in mat3 v_tbnMatrix;


uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform int isTextured;
uniform vec3 cameraPosition;

uniform point_light light0;
uniform material material0;


void main()
{	
	vec3 normal;
	// if (textureSize(normalMap, 0).x > 0){
	// 	vec3 fragNormal = v_tbnMatrix * normalize( texture( normalMap, v_texCoords ).xyz );// * 2.0 - 2.0;			
	// 	normal = fragNormal;		
	// }
	// else {
	// 	normal = normalize(v_normal); 	
	// }
	normal = normalize(v_normal);
	vec4 diffuse_color = vec4(0,0,0,1);
	vec4 specular_color = vec4(0,0,0,1);
	vec3 light_direction = light0.Position - v_fragPosition;
	float dist_to_light = length(light_direction);
	light_direction = normalize(light_direction);

	vec3 ambient_factor = light0.Color * material0.ka * light0.AmbientIntensity;
	vec4 ambient_color = vec4(ambient_factor, 1.0f);

	float diffuse_factor = max(dot(normal, light_direction), 0.0);
	if (diffuse_factor > 0) {
		vec3 diff = light0.Color * material0.kd * diffuse_factor;
		diffuse_color = vec4(diff, 1.0f);
	}

	vec3 vert_to_eye = normalize(cameraPosition - v_fragPosition);
	vec3 reflection = normalize(reflect(light_direction, normal));
	float specular_factor = max(dot(reflection, -vert_to_eye), 0.0);
	if (specular_factor > 0){
		vec3 spec = light0.Color * material0.ks * pow(specular_factor, material0.ns);
		specular_color = vec4(spec, 1.0f);
	}

	vec4 map_color = texture(colorMap, v_texCoords);
	//fcolor = vec4(normal, 1.0);
	//fcolor = ambient_color;	
	//vec3 norm = vec3(abs(normal.x), abs(normal.y), abs(normal.z));
	//fcolor = vec4(norm, 1.0);
	//fcolor = diffuse_color;
	//fcolor = map_color * diffuse_color;
	//fcolor = specular_color;
	fcolor = map_color * (ambient_color + diffuse_color + specular_color);// + vec4(ambient_color.xyz, 1.0) + vec4(specular_color.xyz, 1.0));

	
	
	

}