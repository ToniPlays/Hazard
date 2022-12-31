#pragma once

#include "UtilityCore.h"
#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "../MetalShaderCompiler.h"

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

    class MetalDescriptorSet
    {
    public:
        MetalDescriptorSet() = default;
        ~MetalDescriptorSet() = default;

        void AddWriteDescriptor(MetalWriteDescriptor writeDescriptor);
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, MetalWriteDescriptor>>& GetDescriptorSets() { return m_WriteDescriptors; }
        MetalWriteDescriptor* GetWriteDescriptor(uint32_t type, uint32_t index) { return &m_WriteDescriptors[type][index]; }
        MetalWriteDescriptor* GetWriteDescriptor(uint32_t type, const std::string& name);

        void UpdateBindings(const std::unordered_map<std::string, MSLBinding> bindings);
        void BindGraphicsResources(MTL::RenderCommandEncoder* encoder);
        void BindComputeResources(MTL::ComputeCommandEncoder* encoder);

    private:
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, MetalWriteDescriptor>> m_WriteDescriptors;
    };
}
#endif
