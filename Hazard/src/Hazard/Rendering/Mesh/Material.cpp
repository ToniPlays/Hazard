#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
    Material::Material()
    {
        SetPipeline(ShaderLibrary::GetPipeline("pbr_static"));
    }

    void Material::SetPipeline(Ref<HazardRenderer::Pipeline> pipeline)
    {
        m_ParameterBuffer.Release();
        m_Pipeline = pipeline;
        
        auto& data = pipeline->GetShader()->GetShaderData();
        
        //Specify u_MaterialConstants as the constant buffer for a material
        for(auto& [set, ranges] : data.PushConstants)
        {
            for(auto& [binding, constant] : ranges)
            {
                if(constant.Name == "u_MaterialConstants")
                    UpdateParameters(constant);
            }
        }
        auto& resources = RenderEngine::GetResources();
        
        for(auto& [set, samplers] : data.ImageSamplers)
        {
            if(set == 0) continue;
            for(auto& [binding, sampler] : samplers)
            {
                if(sampler.Dimension == 2)
                    m_Textures[sampler.Name] = resources.WhiteTexture;
            }
        }
    }
    void Material::UpdateParameters(const HazardRenderer::ShaderPushConstantRange& constant)
    {
        m_ParameterBuffer.Allocate(constant.Size);
        m_ParameterBuffer.ZeroInitialize();
        
        m_Params.clear();
        for(auto& member : constant.Members)
            m_Params[member.Name] = { member.Type, member.Offset };
    }
}
