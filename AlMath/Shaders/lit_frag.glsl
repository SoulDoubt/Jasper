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

in vec2 outTexCoords;
in vec3 outNormal;
in vec4 outVertColor;
in vec3 fragPosition;
in mat3 tbnMatrix;
in vec3 lightDirection;

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform int isTextured;
uniform vec3 cameraPosition;

uniform point_light light0;
uniform material material0;

void main()
{	
	if (textureSize(normalMap, 0).x > 0){
		vec3 fragNormal = tbnMatrix * normalize( texture( normalMap, outTexCoords ).xyz ) * 2.0 - 2.0;

		float lambert = max( 0.0, dot( fragNormal, lightDirection ) );

		fcolor = vec4(fragNormal, 1.0);
	}
	else {
		vec3 normal = normalize(outNormal);
		vec3 surfaceToLight = normalize(light0.Position - fragPosition);
		vec3 surfaceToCamera = normalize(cameraPosition - fragPosition);
		float distanceToLight = length(light0.Position - fragPosition);
		vec3 incidenceVector = -surfaceToLight;
		vec3 reflectionVector = reflect(incidenceVector, normal);
		
		vec4 surfaceColor = texture(colorMap, outTexCoords);
		vec3 surfaceRgb = surfaceColor.rgb;

		float attenuation = 1.0 / (1.0 * light0.ConstAtten * pow(distanceToLight, 2));

		vec3 ambientColor = light0.AmbientIntensity * light0.Color * material0.ka * surfaceRgb;

		float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
		vec3 diffuseColor = diffuseCoefficient * surfaceRgb * material0.kd * light0.Color;

		float lambert = max(0.0, dot(surfaceToCamera, reflectionVector));

		float specularCoefficient = pow(lambert, material0.ns);

		vec3 specularColor = specularCoefficient * material0.ks * light0.Color;

		vec3 linearColor = ambientColor + attenuation * (diffuseColor + specularColor);

		vec3 gamma = vec3(1.0/2.2);
		vec3 finalColor = pow(linearColor, gamma);

		fcolor = vec4(finalColor, surfaceColor.a);
	}
	
	//fcolor = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	// vec3 light_direction = eyeSpaceVertPosition - light0.Position;
	// float lightDistance = length(light_direction);
	// light_direction = normalize(light_direction);

	// vec3 view_direction = normalize(cameraPosition - eyeSpaceVertPosition);

	// vec3 norm = normalize(outNormal);	
	
	// vec3 ambient_color = light0.Color * material0.ka * light0.AmbientIntensity;

	// float diff = max(dot(norm, light_direction), 0.0);
	// diff /= 1.0 * (lightDistance * lightDistance);
	// vec3 diffuse_color = light0.Color * light0.DiffuseIntensity * (diff * material0.kd);

	
	// vec3 reflection_direction = reflect(-light_direction, norm);
	// float reflectionComponent = pow(max(dot(view_direction, reflection_direction), 0.0), material0.ns);
	// vec3 specular_color = light0.Color * (reflectionComponent * material0.ks);
	
	// vec3 gamma = vec3(1.0 / 2.2);
	// vec4 texture_color = texture(colorMap, outTexCoords);

	// vec3 lighting_contribution = ambient_color + diffuse_color + specular_color;

	// vec4 col = texture_color * vec4(lighting_contribution, 1.0);				 
	// fcolor =  col;

}