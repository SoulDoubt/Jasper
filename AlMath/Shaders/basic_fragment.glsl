#version 130

out vec4 fcolor;
smooth in vec2 outTexCoords;
smooth in vec3 outNormal;
smooth in vec3 eyeSpaceVertPosition;

uniform sampler2D colorMap;

void main(){	
	fcolor = vec4(1.0, 0.0, 0.0, 1.0);
}