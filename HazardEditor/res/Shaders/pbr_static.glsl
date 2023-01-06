#type Vertex
#version 450

#include "Uniforms/CameraUniform.glslh"
#include "Inputs/InputPBR.glslh"

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
    mat4 transform = mat4(
            vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
            vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
            vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
            vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
        );
    
    
	vec4 worldPosition = transform * vec4(a_Position, 1.0);
	gl_Position = u_Camera.ViewProjection * worldPosition;

	Output.Color = a_Color;
    Output.Normal = mat3(transform) * a_Normal;
    Output.WorldNormal = mat3(transform) * mat3(a_Tangent, a_Binormal, a_Normal);
    Output.WorldPosition = worldPosition.xyz;
    Output.TextureCoords = a_TextureCoords;
    
    v_EntityID = a_EntityID;
}

#type Fragment
#version 450

#include "Uniforms/LightSources.glslh"
#include "Uniforms/UtilityUniform.glslh"

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

layout(push_constant) uniform PushConstants
{
    float Metalness;
    float Roughness;
    vec4 AlbedoColor;
    vec3 Unused;
    bool UseNormalMap;
} u_MaterialConstants;

layout(set = 0, binding = 1) uniform samplerCube u_RadianceMap;
layout(set = 0, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 0, binding = 3) uniform sampler2D u_BRDFLut;

layout(set = 1, binding = 0) uniform sampler2D u_AlbedoMap;
layout(set = 1, binding = 1) uniform sampler2D u_NormalMap;

#include "Utils/Common.glslh"
#include "Utils/Lighting.glslh"
#include "Utils/PostProcessing.glslh"

layout(location = 0) out vec4 OutputColor;
layout(location = 1) out int EntityID;

const float gamma = 2.2;

void main() 
{
    //Initialize Params
    m_Params.Albedo = u_MaterialConstants.AlbedoColor.rgb * Input.Color.rgb * texture(u_AlbedoMap, Input.TextureCoords).rgb;
    m_Params.Metalness = u_MaterialConstants.Metalness;
    m_Params.Roughness = max(u_MaterialConstants.Roughness, 0.05);
    
    if(u_MaterialConstants.UseNormalMap)
    {
        m_Params.Normal = normalize(texture(u_NormalMap, Input.TextureCoords).rgb * 2.0f - 1.0f);
        m_Params.Normal = normalize(Input.WorldNormal * m_Params.Normal);
    }
    else m_Params.Normal = normalize(Input.Normal);
    
    m_Params.View = normalize(u_Util.CameraPos.xyz - Input.WorldPosition);
    m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);

    //Specular reflect direction
    vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
    //Dielectric
    vec3 F0 = mix(vec3(0.04), m_Params.Albedo, m_Params.Metalness);

    vec3 Lo = vec3(0.0);
    //Reflectance
    for(int i = 0; i < u_Lights.DirectionalLightCount; i++)
    {
        //Lighting
        DirectionalLight light = u_Lights.DirectionalLights[i];
        if(light.Color.a <= 0.0)
            continue;
        
        Lo += CalculateDirectionalLight(F0, light);
    }

    vec3 ibl = IBL(F0, Lr) * u_Lights.SkyLightIntensity;
	
    //Tonemapping
    vec3 color = ACESTonemap(ibl + Lo);
    color = GammaCorrect(color, gamma);
    OutputColor = vec4(color, 1.0);
    
    EntityID = v_EntityID;
}
