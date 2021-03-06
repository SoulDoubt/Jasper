#version 330

struct point_light{
	vec3 Color;
	vec3 Position;
	float AmbientIntensity;
	float DiffuseIntensity;	
	
	float ConstAtten;
	float LinearAtten;
	float ExpAtten;
	float Radius;
};

struct material{
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float ns;
};

struct directional_light{
	vec3 Color;
	vec3 Direction;
	float AmbientIntensity;
	float DiffuseIntensity;	
};

out vec4 fcolor;

smooth in vec2 v_texCoords;
smooth in vec3 v_normal;
smooth in vec4 v_vertColor;
smooth in vec4 v_fragPosition;
in mat3 v_tbnMatrix;


uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform int isTextured;
uniform vec3 cameraPosition;

uniform point_light plight0;
uniform directional_light dlight0;
uniform material material0;

vec4 CalculatePointLight(point_light plight, vec3 normal, vec3 specular){

	vec4 diffuse_color = vec4(0,0,0,1);
	vec4 specular_color = vec4(0,0,0,1);
	vec3 light_direction = v_fragPosition.xyz - plight.Position;
	float dist_to_light = length(light_direction);
	light_direction = normalize(light_direction);

	vec3 ambient_factor = plight.Color * material0.ka * plight.AmbientIntensity;
	vec4 ambient_color = vec4(ambient_factor, 1.0f);

	float diffuse_factor = max(dot(-light_direction, normal), 0.0);
	if (diffuse_factor > 0) {
		vec3 diff = plight.Color * material0.kd * plight.DiffuseIntensity * diffuse_factor;
		diffuse_color = vec4(diff, 1.0f);
		vec3 vert_to_eye = normalize(v_fragPosition.xyz - cameraPosition);
		vec3 reflection = normalize(reflect(light_direction, normal));
		float specular_factor = max(dot(reflection, -vert_to_eye), 0.0);
		if (specular_factor > 0){
			vec3 spec = plight.Color * specular * pow(specular_factor, material0.ns);
			specular_color = vec4(spec, 1.0f);
		}
	}

	float d = max(dist_to_light - plight.Radius, 0.0f);
	float dnom = d/plight.Radius + 1;
	float attenuation = 1 / (dnom * dnom);
	attenuation = max(attenuation, 0.0f);


	return (ambient_color + diffuse_color + specular_color) * attenuation;	
}

vec4 CalculateDirectionalLight(directional_light dlight, vec3 normal, vec3 specular){
	vec4 diffuse_color = vec4(0,0,0,1);
	vec4 specular_color = vec4(0,0,0,1);
	vec3 light_direction = dlight.Direction;
	light_direction = normalize(light_direction);

	vec3 ambient_factor = dlight.Color * material0.ka * dlight.AmbientIntensity;
	vec4 ambient_color = vec4(ambient_factor, 1.0f);

	float diffuse_factor = max(dot(-light_direction, normal), 0.0);
	if (diffuse_factor > 0) {
		vec3 diff = dlight.Color * material0.kd * dlight.DiffuseIntensity * diffuse_factor;
		diffuse_color = vec4(diff, 1.0f);
		vec3 vert_to_eye = normalize(v_fragPosition.xyz - cameraPosition);
		vec3 reflection = normalize(reflect(-light_direction, normal));
		float specular_factor = max(dot(reflection, vert_to_eye), 0.0);
		if (specular_factor > 0){
			vec3 spec = dlight.Color * specular * pow(specular_factor, material0.ns);
			specular_color = vec4(spec, 1.0f);
		}
	}
	return ambient_color + diffuse_color + specular_color;	
}


void main()
{	
	vec3 normal;
	if (textureSize(normalMap, 0).x > 0){
		vec3 fn = texture( normalMap, v_texCoords ).xyz;
		fn.xy = fn.xy * 2.0 - 1.0;
		//fn = 128.0/255.0 * fn - vec3(1.0, 1.0, 1.0);		
		vec3 fragNormal = v_tbnMatrix * fn;
		//fragNormal.x = (fragNormal.x * 2.0) -1;
		//fragNormal.y = (fragNormal.y * 2.0) -1;
		//fragNormal.z = (fragNormal.z * 2.0) -1;
		normal = fragNormal;			
	}
	else {
		normal = normalize(v_normal); 	
	}
	//normal = normalize(v_normal);
	
	vec4 map_color = texture(colorMap, v_texCoords);
	vec3 materialSpecular = vec3(0,0,0);
	if (textureSize(specularMap, 0).x > 0){
		materialSpecular = texture( specularMap, v_texCoords ).xyz;
	}
	else{
		materialSpecular = material0.ks;
	}
	if (map_color == vec4(0,0,0,0)){
	 	map_color = vec4(material0.kd, 1.0);
	}
	vec4 lighting = vec4(0,0,0,1);
	lighting += CalculatePointLight(plight0, normal, materialSpecular);
	lighting += CalculateDirectionalLight(dlight0, normal, materialSpecular);
	

	//float attenuation = plight0.ConstAtten + plight0.LinearAtten * dist_to_light + plight0.ExpAtten * dist_to_light * dist_to_light;

	
	//fcolor = vec4(normal, 1.0);
	//fcolor = ambient_color;	
	//vec3 norm = vec3(abs(normal.x), abs(normal.y), abs(normal.z));
	//fcolor = vec4(normal, 1.0);
	//fcolor = diffuse_color;
	//fcolor = map_color * diffuse_color;
	//fcolor = specular_color;
	fcolor = map_color * lighting;// + vec4(ambient_color.xyz, 1.0) + vec4(specular_color.xyz, 1.0));
	//fcolor = pow(fcolor, 2.2);
	

		

}