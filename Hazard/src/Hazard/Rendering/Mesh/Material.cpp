#include "Material.h"
#include "ShaderLibrary.h"

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
        
        auto& pushConstants = pipeline->GetShader()->GetShaderData().PushConstants;
        
        //Specify u_MaterialConstants as the constant buffer for a material
        for(auto& [set, ranges] : pushConstants)
        {
            for(auto& [binding, constant] : ranges)
            {
                if(constant.Name == "u_MaterialConstants")
                {
                    UpdateParameters(constant);
                    return;
                }
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
