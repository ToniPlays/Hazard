#type Vertex
#version 450

#include "Uniforms/CameraUniform.glslh"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;
layout(location = 5) in vec2 a_TextureCoords;

struct VertexOuput
{
    vec4 Color;
    vec3 WorldPosition;
    vec3 Normal;
    vec2 TextureCoords;
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
    vec2 TextureCoords;
    mat3 WorldNormal;
};

layout(location = 0) in VertexOuput Input;
layout(location = 7) in flat int v_EntityID;

#include "Utils/Common.glslh"

layout(location = 0) out vec4 OutputColor;
layout(location = 1) out uint EntityID;

const float gamma = 2.2;

void main() 
{
    OutputColor = Input.Color;
    EntityID = v_EntityID;
}
