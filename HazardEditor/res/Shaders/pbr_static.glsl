#type Vertex
#version 450

#include "Uniforms/CameraUniform.glslh"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;
layout(location = 5) in vec3 a_TextureCoords;

struct VertexOuput
{
    vec4 Color;
    vec3 WorldPosition;
    vec3 Normal;
    vec3 TextureCoords;
    mat3 WorldNormal;
};

layout(push_constant) uniform Transform
{
    mat4 Transform;
} p_Transform;

layout(location = 0) out VertexOuput Output;
layout(location = 7) out int v_EntityID;

void main()
{
	vec4 worldPosition = p_Transform.Transform * vec4(a_Position, 1.0);
	gl_Position = u_Camera.ViewProjection * worldPosition;

	Output.Color = a_Color;
    Output.Normal = a_Normal;
    Output.WorldNormal = mat3(p_Transform.Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
    Output.WorldPosition = worldPosition.xyz;
    Output.TextureCoords = a_TextureCoords;
    
    v_EntityID = 0;
}

#type Fragment
#version 450

struct VertexOuput
{
    vec4 Color;
    vec3 WorldPosition;
    vec3 Normal;
    vec3 TextureCoords;
    mat3 WorldNormal;
};

layout(location = 0) in VertexOuput Input;
layout(location = 7) in flat int v_EntityID;

struct PBRParameters
{
	vec3 Albedo;
	float Roughness;
	float Metalness;

	vec3 Normal;
	vec3 View;
	float NdotV;
	int Flags;
} m_Params;

#include "Uniforms/CameraUniform.glslh"

layout(location = 0) out vec4 OutputColor;
layout(location = 1) out uint EntityID;

#include "Utils/Common.glslh"
#include "Utils/PostProcessing.glslh"

const float gamma = 2.2;

void main() 
{
    vec3 color = ACESTonemap(Input.Color.rgb);
    color = GammaCorrect(color, gamma);
    OutputColor = vec4(color, 1.0);
    EntityID = v_EntityID;
}
