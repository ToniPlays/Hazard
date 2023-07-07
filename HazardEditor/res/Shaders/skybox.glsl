#type Vertex
#version 450

#include "Uniforms/CameraUniform.glslh"

layout(location = 0) out vec3 v_Position;

vec3 quadPos[6] = vec3[](
	vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
	vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
);

void main() 
{
	vec4 position = vec4(quadPos[gl_VertexIndex].xy, 1.0, 1.0);
	gl_Position = position;

	vec4 inversedPos = position;
	v_Position = (mat4(mat3(inverse(u_Camera.View))) * inverse(u_Camera.Projection) * inversedPos).xyz;
}

#type Fragment
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 0) out vec4 OutputColor;
layout(location = 1) out uint EntityID;

void main() 
{
	vec4 color = vec4(0.5, 0.5, 0.9, 1.0);
	OutputColor = vec4(color.rgb, 1.0);
	EntityID = 0;
}
