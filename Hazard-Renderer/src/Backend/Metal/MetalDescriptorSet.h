#pragma once

#include "UtilityCore.h"
#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "MetalShaderCompiler.h"
#include "Backend/Core/DescriptorSet.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>


namespace HazardRenderer::Metal
{
    enum MetalDescriptorTypeFlags
    {
        MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        MTL_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE,
        MTL_DESCRIPTOR_TYPE_PUSH_CONSTANT,
        MTL_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        MTL_DESCRIPTOR_TYPE_MAX_ENUM,
        MTL_DESCRIPTOR_TYPE_MAX_FLAGS
    };

    struct MetalWriteDescriptor
    {
        std::string DebugName;
        MetalDescriptorTypeFlags Type = MTL_DESCRIPTOR_TYPE_MAX_FLAGS;
        uint32_t Binding = UINT32_MAX;
        uint32_t SecondaryBinding = UINT32_MAX;
        uint32_t ArraySize = 0;
        uint32_t Dimension = 1;
        uint32_t ActualBinding = UINT32_MAX;
        uint32_t Size = 0;
        uint32_t Flags = 0;
        Buffer Buffer;
        std::unordered_map<uint32_t, Ref<RefCount>> BoundValue;
    };

    class MetalDescriptorSet : public DescriptorSet
    {
    public:
        MetalDescriptorSet(DescriptorSetCreateInfo* info);
        ~MetalDescriptorSet() = default;

        void Write(uint32_t binding, uint32_t index, Ref<Image> image, Ref<Sampler> sampler, bool updateAll = false) override;
        void Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll = false) override;
        
        void Invalidate();
        void Invalidate_RT();
        
        //Metal specific
        void BindGraphicsResources(MTL::RenderCommandEncoder* encoder);
        void BindComputeResources(MTL::ComputeCommandEncoder* encoder);

    private:
        
        struct DescriptorValue {
            Ref<RefCount> Value;
            Ref<RefCount> Secondary;
        };
        
        std::string m_DebugName;
        uint32_t m_Set;
        DescriptorSetLayout m_Layout;
        std::unordered_map<uint32_t, DescriptorValue> m_DescriptorValues;
    };
}
#endif
