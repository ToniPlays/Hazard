#version 450 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoords;

out vec2 f_coords;


void main()
{
	f_coords = texCoords;
	gl_Position = vec4(pos, 1.0, 1.0);
}