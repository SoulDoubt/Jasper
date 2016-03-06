#version 130

in vec3 inPosition;
in vec2 inTexCoords;

smooth out vec2 tex_coords;

void main(){
	gl_Position = vec4(inPosition.xy, 0.0, 1.0);
	tex_coords = inTexCoords;
}