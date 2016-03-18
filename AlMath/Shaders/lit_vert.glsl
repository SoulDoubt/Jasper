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

in vec3 position;
in vec2 texCoords;
in vec4 color;
in vec3 normal;
in vec3 tangent;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;

out vec2 outTexCoords;
out vec3 outNormal;
out vec4 outVertColor;
out vec3 fragPosition;
out vec3 lightDirection;
out mat3 tbnMatrix;

void main()
{
	outVertColor = color;
	outTexCoords = texCoords;
	fragPosition = ( modelMatrix * vec4( position, 1.0f ) ).xyz;
	outNormal = normalMatrix * normal;

	vec3 n = normalize( ( modelMatrix * vec4(normal, 0.0) ) ).xyz;
	vec3 t = normalize( ( modelMatrix * vec4(tangent, 0.0) ) ).xyz;
	vec3 b = normalize( ( modelMatrix * vec4(cross(normal, tangent), 0.0) ) ).xyz;
	tbnMatrix = mat3( t, b, n );

	lightDirection = normalize(lightPosition - fragPosition);

	gl_Position = mvpMatrix * vec4( position, 1.0f );

}

