#version 450 core
layout (location = 0) in vec3 pos;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 WorldPos;

void main()
{
    WorldPos = pos;
    gl_Position =  u_Projection * u_View * vec4(WorldPos, 1.0);
}