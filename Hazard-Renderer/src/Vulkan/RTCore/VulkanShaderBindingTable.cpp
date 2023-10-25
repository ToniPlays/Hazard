
#include "VulkanShaderBindingTable.h"

#ifdef HZR_INCLUDE_VULKAN
#include "Core/GraphicsContext.h"
#include "Core/Renderer.h"

#include "Vulkan/Core/VulkanAllocator.h"
#include "Vulkan/VulkanContext.h"

namespace HazardRenderer::Vulkan
{
    static PFN_vkGetRayTracingShaderGroupHandlesKHR fpGetRayTracingShaderGroupHandlesKHR;

    VulkanShaderBindingTable::VulkanShaderBindingTable(ShaderBindingTableCreateInfo* createInfo)
    {
        auto device = VulkanContext::GetInstance()->GetLogicalDevice();
        GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetRayTracingShaderGroupHandlesKHR);

        m_Pipeline = createInfo->pPipeline.As<VulkanPipeline>();

        Ref<VulkanShaderBindingTable> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
            });
    }
    void VulkanShaderBindingTable::Invalidate_RT()
    {
        CreateBindingTableBuffers();
    }
    void VulkanShaderBindingTable::CreateBindingTableBuffers()
    {
        auto device = VulkanContext::GetInstance()->GetLogicalDevice();
        const auto& limits = device->GetPhysicalDevice()->GetDeviceLimits();

        const uint32_t groupCount = m_Pipeline->GetShaderGroups().size();
        const uint32_t sbtSize = groupCount * limits.ShaderGroupHandleSize;

        std::vector<uint8_t> shaderHandleStorage(sbtSize);

        VK_CHECK_RESULT(fpGetRayTracingShaderGroupHandlesKHR(device->GetVulkanDevice(), m_Pipeline->GetVulkanPipeline(), 0, groupCount, sbtSize, shaderHandleStorage.data()), "Failed to get Group handles");

        VulkanAllocator allocator("VulkanShaderBindingTable");

        //Raygen
        {
            BindingTableBuffer buffer = {};
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = limits.ShaderGroupHandleSize;
            bufferInfo.usage = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
            buffer.Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, buffer.Buffer);


            uint8_t* data = allocator.MapMemory<uint8_t>(buffer.Allocation);
            memcpy(data, shaderHandleStorage.data(), limits.ShaderGroupHandleSize);
            allocator.UnmapMemory(buffer.Allocation);

            buffer.StridedAddress.deviceAddress = VkUtils::GetBufferAddress(buffer.Buffer).deviceAddress;
            buffer.StridedAddress.stride = limits.ShaderGroupHandleSize;
            buffer.StridedAddress.size = limits.ShaderGroupHandleSize;

            m_BindingTableBuffers[SHADER_STAGE_RAYGEN_BIT] = buffer;
        }
        //Miss
        {
            BindingTableBuffer buffer = {};
            VkBufferCreateInfo bufferInfo = {};

            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = limits.ShaderGroupHandleSize;
            bufferInfo.usage = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
            buffer.Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, buffer.Buffer);

            uint8_t* data = allocator.MapMemory<uint8_t>(buffer.Allocation);
            memcpy(data, shaderHandleStorage.data() + limits.ShaderGroupHandleSize, limits.ShaderGroupHandleSize);
            allocator.UnmapMemory(buffer.Allocation);

            buffer.StridedAddress.deviceAddress = VkUtils::GetBufferAddress(buffer.Buffer).deviceAddress;
            buffer.StridedAddress.stride = limits.ShaderGroupHandleSize;
            buffer.StridedAddress.size = limits.ShaderGroupHandleSize;

            m_BindingTableBuffers[SHADER_STAGE_MISS_BIT] = buffer;
        }
        //Closest hit
        {
            BindingTableBuffer buffer = {};
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = limits.ShaderGroupHandleSize;
            bufferInfo.usage = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
            buffer.Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, buffer.Buffer);


            uint8_t* data = allocator.MapMemory<uint8_t>(buffer.Allocation);
            memcpy(data, shaderHandleStorage.data() + limits.ShaderGroupHandleSize * 2, limits.ShaderGroupHandleSize);
            allocator.UnmapMemory(buffer.Allocation);

            buffer.StridedAddress.deviceAddress = VkUtils::GetBufferAddress(buffer.Buffer).deviceAddress;
            buffer.StridedAddress.stride = limits.ShaderGroupHandleSize;
            buffer.StridedAddress.size = limits.ShaderGroupHandleSize;

            m_BindingTableBuffers[SHADER_STAGE_CLOSEST_HIT_BIT] = buffer;
        }
    }
}
#endif
