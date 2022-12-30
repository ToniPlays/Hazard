#pragma once

#include "Hazard/Assets/Asset.h"
#include "Backend/Core/Pipeline/Pipeline.h"

namespace Hazard
{
    class Material : public Asset
    {
    public:
        
        Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; };
        
    private:
        Ref<HazardRenderer::Pipeline> m_Pipeline;
        
    };
}
