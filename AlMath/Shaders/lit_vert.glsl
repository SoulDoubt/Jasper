#version 330

struct Transform {
	vec3 position;
	vec4 orientation;
	vec3 scale;
};

vec3 quaternionRotate(vec4 quat, vec3 vec){
	vec3 t = 2.0 * cross(quat.xyz, vec);
	return (vec + quat.w * t + cross(quat.xyz, t));
}

in vec3 position;
in vec2 texCoords;
in vec4 color;
in vec3 normal;

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvMatrix;
uniform mat4 modelMatrix;

smooth out vec2 outTexCoords;
smooth out vec3 outNormal;
smooth out vec4 outVertColor;
smooth out vec3 fragPosition;

uniform Transform transform;


void main()
{
	outVertColor = color;
	outTexCoords = texCoords;
	fragPosition = (modelMatrix * vec4(position, 1.0)).xyz;
	outNormal = normalMatrix * normal;
	gl_Position = mvpMatrix * vec4(position, 1.0);

}

