#version 330

in vec3 position;
in vec2 texCoords;
in vec4 color;
in vec3 normal;
in vec4 tangent;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;

out vec2 v_texCoords;
out vec3 v_normal;
out vec4 v_vertColor;
out vec3 v_fragPosition;
out vec3 v_lightDirection;
out mat3 v_tbnMatrix;
out vec3 v_lightPosition;

void main()
{
	v_vertColor = color;
	v_texCoords = texCoords;
	v_fragPosition = ( modelMatrix * vec4( position, 1.0f ) ).xyz;
	v_normal = normalMatrix * normal;

	vec3 n = normalize( ( (modelMatrix) * vec4(normal, 0.0) ) ).xyz;
	vec3 b = normalize( ( (modelMatrix) * vec4(cross(normal, tangent.xyz) * tangent.w, 0.0) ) ).xyz;
	vec3 t = normalize( ( (modelMatrix) * vec4(tangent.xyz, 0.0) ) ).xyz;
	v_tbnMatrix = mat3( t, b, n );

	vec3 lp = (modelMatrix * vec4(lightPosition, 1.0)).xyz;

	v_lightDirection = normalize(lightPosition - v_fragPosition);
	v_lightPosition = lightPosition;

	gl_Position = mvpMatrix * vec4( position, 1.0f );

}

