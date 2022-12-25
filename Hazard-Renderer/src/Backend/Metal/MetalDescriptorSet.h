#pragma once

#include "UtilityCore.h"
#include "Backend/Core/Core.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hPP>

namespace HazardRenderer::Metal
{
    enum MetalDescriptorTypeFlags
    {
        MTL_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        MTL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        MTL_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        MTL_DESCRIPTOR_TYPE_MAX_ENUM,
        MTL_DESCRIPTOR_TYPE_MAX_FLAGS
    };

    struct MetalWriteDescriptor
    {
        std::string DebugName;
        MetalDescriptorTypeFlags Type = MTL_DESCRIPTOR_TYPE_MAX_FLAGS;
        uint32_t Binding = UINT32_MAX;
        uint32_t ArraySize = 0;
        uint32_t Dimension = 1;
        uint32_t ActualBinding = UINT32_MAX;
        std::unordered_map<uint32_t, Ref<RefCount>> BoundValue;
    };

    class MetalDescriptorSet
    {
    public:
        MetalDescriptorSet() = default;
        ~MetalDescriptorSet() = default;

        void AddWriteDescriptor(MetalWriteDescriptor writeDescriptor);
        std::unordered_map<uint32_t, MetalWriteDescriptor>& GetDescriptorSets() { return m_WriteDescriptors; }
        MetalWriteDescriptor* GetWriteDescriptor(uint32_t index) { return &m_WriteDescriptors[index]; }
        MetalWriteDescriptor* GetWriteDescriptor(const std::string& name);

        void BindResources(MTL::RenderCommandEncoder* encoder, bool isCompute = false);

    private:
        std::unordered_map<uint32_t, MetalWriteDescriptor> m_WriteDescriptors;
    };
}
