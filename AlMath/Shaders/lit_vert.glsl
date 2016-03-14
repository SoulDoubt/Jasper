#version 330


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

void main()
{
	outVertColor = color;
	outTexCoords = texCoords;
	fragPosition = (modelMatrix * vec4(position, 1.0f)).xyz;
	outNormal = normalMatrix * normal;
	gl_Position = mvpMatrix * vec4(position, 1.0f);

}

