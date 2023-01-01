#pragma once

#include "Hazard/Assets/Asset.h"
#include "Backend/Core/Pipeline/Pipeline.h"

#include "UtilityCore.h"

namespace Hazard
{
    struct MaterialParam
    {
        uint32_t Offset;
    };

    class Material : public Asset
    {
    public:
        
        template<typename T>
        void Set(const std::string& key, T& value) {};
        
        template<typename T>
        T Get(const std::string& key) { return T(); };
        
        Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; };
    private:
        Ref<HazardRenderer::Pipeline> m_Pipeline;
        std::unordered_map<std::string, MaterialParam> m_Params;
        
    };
}
