#type Vertex
#version 450

#include "Uniforms/CameraUniform.glslh"
#include "Uniforms/ModelUniform.glslh"
#include "Inputs/InputPBR.glslh"

layout(location = 0) out vec4 Color;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 WorldPosition;

void main()
{
    vec4 worldPosition = u_Model.Transform * vec4(a_Position.xyz, 1.0);
    gl_Position = u_Camera.ViewProjection * worldPosition;

    Color = a_Color;
    Normal = mat3(u_Model.Transform) * a_Normal.xyz;
    WorldPosition = worldPosition.xyz;
    
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glslh"
#include "Uniforms/LightSources.glslh"
#include "Uniforms/ModelUniform.glslh"
#include "Uniforms/UtilityUniform.glslh"

layout(location = 0) in vec4 Color;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 WorldPosition;

layout(set = 1, binding = 1) uniform samplerCube u_RadianceMap;

layout(set = 1, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 1, binding = 3) uniform sampler2D u_BRDFLut;


#include "Utils/Common.glslh"
#include "Utils/Lighting.glslh"
#include "Utils/PostProcessing.glslh"

layout(location = 0) out vec4 OutputColor;

void main()
{
    OutputColor = Color;
}

