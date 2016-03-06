#version 130

smooth in vec2 tex_coords;
uniform sampler2D colorMap;
out vec4 fcolor;

void main(){
	fcolor = vec4(1.0f, 1.0f, 1.0f, texture(colorMap, tex_coords).r);	
}