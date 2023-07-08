#type Vertex
#version 450

#include "../Uniforms/CameraUniform.glslh"
#include "../Inputs/InputCircle.glslh"

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec3 f_LocalPosition;
layout(location = 2) out float f_Thickness;
layout(location = 3) out float f_Fade;

layout(push_constant) uniform Transform
{
    mat4 Transform;
} p_Transform;

void main() 
{
	f_Color = a_Color;
	f_LocalPosition = a_LocalPosition;
	f_Thickness = a_Thickness;
	f_Fade = a_Fade;
	gl_Position = u_Camera.ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 f_LocalPosition;
layout(location = 2) in float f_Thickness;
layout(location = 3) in float f_Fade;

layout(set = 0, binding = 1) uniform samplerCube u_RadianceMap;
layout(set = 0, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 0, binding = 3) uniform sampler2D u_BRDFLut;

layout(location = 0) out vec4 color;
layout(location = 1) out uint EntityID;

void main() 
{

	float distance = 1.0 - length(f_LocalPosition);
	float circle = smoothstep(0.0, f_Fade, distance);

	color = vec4(distance, distance, distance, 1.0);
	circle *= smoothstep(f_Thickness + f_Fade, f_Thickness, distance);

	if(circle == 0.0)
		discard;

	color = f_Color;
	color.a *= circle;
}
