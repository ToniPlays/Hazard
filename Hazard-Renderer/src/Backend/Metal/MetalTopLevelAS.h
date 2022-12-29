#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    //class MetalBottomLevelAS;

    class MetalTopLevelAS : public TopLevelAS
    {
    public:
        MetalTopLevelAS(AccelerationStructureCreateInfo* info);
        ~MetalTopLevelAS() = default;
        
        size_t GetCount() const override { return 0; };
        void PushInstances(const glm::mat4& transform, Ref<AccelerationStructure> accelerationStructure) override {};
        
        void Build(const BuildType& type) override {};
        void Build(MTL::AccelerationStructureCommandEncoder* encoder, const BuildType& type) {}

        void Invalidate() override;
        void Invalidate_RT();
        
        //Metal specific
        
        MTL::AccelerationStructure* GetMetalStructure() const { return m_Structure; }

       
    private:
        void CreateTopLevel() {};
        void CreateInstanceBuffer() {};

    private:
        std::string m_DebugName;
        AccelerationStructureLevel m_Level;

        std::vector<AccelerationStructureInstance> m_Instances;
        MTL::AccelerationStructure* m_Structure;
    };
}
#endif

