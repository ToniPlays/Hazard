#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 2) in vec4 a_MRow0;
layout(location = 3) in vec4 a_MRow1;
layout(location = 4) in vec4 a_MRow2;

layout(location = 0) out vec4 v_Color;

layout(set = 0, binding = 0) uniform Camera
{
    uniform mat4 Projection;
} u_Camera;


void main()
{
    mat4 transform = mat4(
            vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
            vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
            vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
            vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
        );
    
    
	vec4 worldPosition = transform * vec4(a_Position, 1.0);
	gl_Position = u_Camera.Projection * worldPosition;

    v_Color = a_Color;
}

#type Fragment
#version 450

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 OutputColor;

void main() 
{
   OutputColor = v_Color;
}
