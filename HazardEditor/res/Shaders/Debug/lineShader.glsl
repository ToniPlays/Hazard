#type Vertex
#version 450

#include "../Uniforms/CameraUniform.glslh"

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;

layout(location = 0) out vec4 f_Color;

void main() 
{
	f_Color = v_Color;
	gl_Position = u_Camera.ViewProjection * vec4(v_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;

layout(set = 0, binding = 1) uniform samplerCube u_RadianceMap;
layout(set = 0, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 0, binding = 3) uniform sampler2D u_BRDFLut;

layout(location = 0) out vec4 color;
layout(location = 1) out uint EntityID;

void main() 
{
	color = f_Color;
	EntityID = 0;
}
