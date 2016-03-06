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

smooth out vec2 outTexCoords;
smooth out vec3 outNormal;
smooth out vec4 outVertColor;
out vec3 eyeSpaceVertPosition;

uniform Transform transform;


void main()
{
	gl_Position = mvpMatrix * vec4(position, 1.0);
	outVertColor = color;
	outTexCoords = texCoords;
	
	eyeSpaceVertPosition = (mvMatrix * vec4(position, 1.0f)).xyz;
	
	outNormal = (normalMatrix * normal).xyz;		
	//vec3 pos = position;
	//pos = transform.position + quaternionRotate(transform.orientation, pos);
	//gl_Position = mvMatrix * vec4(pos, 1.0);
	//gl_Position = vec4(pos, 1.0);

}

