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

layout(location = 0) out VertexOuput Output;
layout(location = 7) out int v_EntityID;

void main()
{
	vec4 worldPosition = vec4(a_Position, 1.0);
	gl_Position = u_Camera.ViewProjection * worldPosition;

	Output.Color = a_Color;
    Output.Normal = a_Normal;
    Output.WorldNormal = mat3(a_Tangent, a_Binormal, a_Normal);
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

layout(push_constant, std140) uniform PushConstants
{
    uniform float Metalness;
    uniform float Roughness;
} u_PushConstants;

#include "Uniforms/CameraUniform.glslh"

layout(location = 0) out vec4 OutputColor;
layout(location = 1) out uint EntityID;

#include "Utils/Common.glslh"
#include "Utils/Lighting.glslh"
#include "Utils/PostProcessing.glslh"

const float gamma = 2.2;

void main() 
{
    m_Params.Albedo = Input.Color.rgb;
    m_Params.Metalness = u_PushConstants.Metalness;
    m_Params.Roughness = max(u_PushConstants.Roughness, 0.05);
    m_Params.Normal = normalize(Input.Normal);

    m_Params.View = normalize(u_Camera.Position.xyz - Input.WorldPosition);
    m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);

    vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
    vec3 F0 = mix(vec3(0.04), m_Params.Albedo, m_Params.Metalness);

    //Light calculations
    vec3 Lo = vec3(0.0);
       
    //Calculate final color
    vec3 ibl = IBL(F0, Lr);

    vec3 color = Input.Color.rgb;
    color = ACESTonemap(ibl + Lo);
    color = GammaCorrect(color, gamma);

    OutputColor = vec4(color, 1.0);
    EntityID = v_EntityID;
}
