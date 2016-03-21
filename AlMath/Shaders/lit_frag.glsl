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

in vec2 v_texCoords;
in vec3 v_normal;
in vec4 v_vertColor;
in vec3 v_fragPosition;
in vec3 v_lightDirection;
in mat3 v_tbnMatrix;
in vec3 v_lightPosition;


uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform int isTextured;
uniform vec3 cameraPosition;

uniform point_light light0;
uniform material material0;

vec4 CalculateLighting(point_light light, vec3 lightDirection, vec3 normal, material mat){
	vec4 ambientColor = vec4(light.Color, 1.0f) * light.AmbientIntensity;
	float diffuseFactor = max(0.0, dot(normal, -lightDirection));

	vec4 diffuseColor = vec4(0,0,0,1);
	vec4 specularColor = vec4(0,0,0,1);

	if (diffuseFactor > 0){
		diffuseColor = vec4(light.Color * light.DiffuseIntensity * diffuseFactor, 1.0f);
		vec3 vertexToEye = normalize(cameraPosition - v_fragPosition);
		vec3 reflection = normalize(reflect(lightDirection, normal));
		float specularFactor = max(0.0, dot(vertexToEye, reflection));
		if (specularFactor > 0){
			specularFactor = pow(specularFactor, mat.ns);
			specularColor = vec4(light.Color * mat.ks * specularFactor, 1.0f);
		}
	}	
	return (ambientColor + diffuseColor + specularColor);
}


void main()
{	
	vec3 normal;
	if (textureSize(normalMap, 0).x > 0){
		vec3 fragNormal = v_tbnMatrix * normalize( texture( normalMap, v_texCoords ).xyz );// * 2.0 - 2.0;		
		//normal = vec3(abs(fragNormal.x), abs(fragNormal.y), abs(fragNormal.z));
		normal = fragNormal;		
	}
	else {
		normal = normalize(v_normal);
	}
	vec4 lightContribution = CalculateLighting(light0, v_lightDirection, normal, material0);
	fcolor = texture(colorMap, v_texCoords) * lightContribution;
	

}