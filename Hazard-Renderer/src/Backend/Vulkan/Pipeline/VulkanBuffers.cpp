
#include "VulkanBuffers.h"
#ifdef HZR_INCLUDE_VULKAN
#include "../VulkanContext.h"
#include "../VulkanRenderCommandBuffer.h"
#include "MathCore.h"
#include "Backend/Core/Renderer.h"


namespace HazardRenderer::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(VertexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;
		m_Usage = info->Usage;

		if (info->Layout != nullptr)
			m_Layout = *info->Layout;

		Ref<VulkanVertexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, data = info->Data, size = info->Size]() mutable {
			VulkanDevice& device = VulkanContext::GetPhysicalDevice();
			VulkanAllocator allocator("VertexBuffer");

			if (data == nullptr) {

				VkBufferCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				createInfo.size = size;
				createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				instance->m_Allocation = allocator.AllocateBuffer(createInfo, VMA_MEMORY_USAGE_GPU_TO_CPU, instance->m_Buffer);
				instance->m_LocalData.Allocate(size);
			}
			else
			{
				instance->m_LocalData = Buffer::Copy(data, size);

				VkBufferCreateInfo stagingCreateInfo = {};
				stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				stagingCreateInfo.size = size;
				stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				VkBuffer stagingBuffer;
				VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

				uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);
				memcpy_s(destData, size, instance->m_LocalData.Data, instance->m_LocalData.Size);
				allocator.UnmapMemory(stagingBufferAlloc);

				VkBufferCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				createInfo.size = size;
				createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

				instance->m_Allocation = allocator.AllocateBuffer(createInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_Buffer);

				VkCommandBuffer copyCmd = device.GetCommandBuffer(true);
				VkBufferCopy copyRegion = {};
				copyRegion.size = instance->m_LocalData.Size;

				vkCmdCopyBuffer(copyCmd, stagingBuffer, instance->m_Buffer, 1, &copyRegion);
				device.FlushCommandBuffer(copyCmd);
				allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);

			}
		});
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		m_LocalData.Release();
		Ref<VulkanVertexBuffer> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			VulkanContext::GetPhysicalDevice().WaitUntilIdle();
			VulkanAllocator allocator("VertexBuffer");
			allocator.DestroyBuffer(instance->m_Buffer, instance->m_Allocation);
			});
	}
	void VulkanVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer, uint32_t binding)
	{
		Ref<VulkanVertexBuffer> instance = this;
		Ref<RenderCommandBuffer> commandBuffer = cmdBuffer;
		Renderer::Submit([instance, commandBuffer, binding]() mutable {
			HZR_ASSERT(commandBuffer->IsRecording(), "CommandBuffer not in recording state");
			uint32_t frameIndex = VulkanContext::GetVulkanSwapchain()->GetCurrentBufferIndex();
			auto vkCmdBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(vkCmdBuffer, binding, 1, &instance->m_Buffer, offsets);
			});
	}
	void VulkanVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		m_LocalData.Write(data, size, 0);
		Ref<VulkanVertexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, data, size]() mutable {
			instance->RT_SetData(data, size);
			});
	}
	void VulkanVertexBuffer::RT_SetData(const void* data, uint32_t size)
	{
		VulkanAllocator allocator("VulkanVertexBuffer");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_Allocation);
		memcpy_s(pData, m_LocalData.Size, (uint8_t*)data, size);
		allocator.UnmapMemory(m_Allocation);
	}


	VulkanIndexBuffer::VulkanIndexBuffer(IndexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;

		if (info->Data == nullptr)
			return;

		SetData(info->Data, info->Size);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		m_LocalData.Release();
		VulkanContext::GetPhysicalDevice().WaitUntilIdle();
		VulkanAllocator allocator("IndexBuffer");
		allocator.DestroyBuffer(m_Buffer, m_Allocation);
	}
	void VulkanIndexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		Ref<VulkanIndexBuffer> instance = this;
		Renderer::Submit([instance, cmdBuffer]() mutable {
			uint32_t frameIndex = VulkanContext::GetVulkanSwapchain()->GetCurrentBufferIndex();
			auto vkCmdBuffer = cmdBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindIndexBuffer(vkCmdBuffer, instance->m_Buffer, 0, VK_INDEX_TYPE_UINT32);
			});
	}
	void VulkanIndexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void VulkanIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		Ref<VulkanIndexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, data, size]() mutable {
			instance->m_Size = size;
			VulkanDevice& device = VulkanContext::GetPhysicalDevice();
			VulkanAllocator allocator("IndexBuffer");
			instance->m_LocalData = Buffer::Copy(data, instance->m_Size);

			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = instance->m_LocalData.Size;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			//Copy data to staging buffer
			uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
			memcpy_s(destData, instance->m_LocalData.Size, instance->m_LocalData.Data, instance->m_LocalData.Size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkBufferCreateInfo indexBufferCreateInfo = {};
			indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferCreateInfo.size = instance->m_LocalData.Size;
			indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			instance->m_Allocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_Buffer);

			VkCommandBuffer copyCmd = device.GetCommandBuffer(true);
			VkBufferCopy copyRegion = {};
			copyRegion.size = instance->m_LocalData.Size;
			vkCmdCopyBuffer(
				copyCmd,
				stagingBuffer,
				instance->m_Buffer,
				1,
				&copyRegion);

			device.FlushCommandBuffer(copyCmd);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
			});
	}




	VulkanUniformBuffer::VulkanUniformBuffer(UniformBufferCreateInfo* createInfo) : m_Size(createInfo->Size),
		m_Binding(createInfo->Binding), m_Usage(createInfo->Usage)
	{
		m_Size = createInfo->Size;

		m_Name = createInfo->Name;
		m_LocalData.Allocate(m_Size * 8);
		m_LocalData.ZeroInitialize();

		RT_Invalidate();
	}
	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		Release();
	}
	void VulkanUniformBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		uint32_t frameIndex = cmdBuffer->GetFrameIndex();
		if (m_FrameIndex == frameIndex) return;

		m_FrameIndex = frameIndex;
		m_Writes = 0;
		m_Offset = 0;

	}
	void VulkanUniformBuffer::Bind_RT(Ref<RenderCommandBuffer> cmdBuffer)
	{
		uint32_t frameIndex = cmdBuffer->GetFrameIndex();
		if (m_FrameIndex == frameIndex) return;

		m_FrameIndex = frameIndex;
		m_Writes = 0;
		m_Offset = 0;

	}
	void VulkanUniformBuffer::Unbind()
	{

	}
	void VulkanUniformBuffer::SetData(const void* data, uint32_t size)
	{
		if (m_Writes * m_Size > m_LocalData.Size)
		{
			ResizeBuffer(m_Writes * m_Size);
		}
		m_LocalData.Write(data, size, m_Writes * m_Size);


		RT_SetData();
		SetCurrentReadPointer(m_Writes, size);

		m_Offset += m_Size * m_Writes >= 1;

		m_Writes++;
	}
	void VulkanUniformBuffer::RT_SetData()
	{
		VulkanAllocator allocator("VulkanUniformBuffer");

		uint8_t* pData = allocator.MapMemory<uint8_t>(m_Allocation);
		memcpy_s(pData, m_LocalData.Size, (const uint8_t*)m_LocalData.Data, m_LocalData.Size);
		allocator.UnmapMemory(m_Allocation);

	}
	void VulkanUniformBuffer::Release()
	{
		VulkanAllocator allocator("UniformBuffer");
		allocator.DestroyBuffer(m_Buffer, m_Allocation);

		m_Buffer = nullptr;
		m_Allocation = nullptr;
		m_LocalData.Release();
	}
	void VulkanUniformBuffer::RT_Invalidate()
	{
		if (m_Allocation)
			Release();

		auto device = VulkanContext::GetPhysicalDevice().GetVulkanDevice();
		uint32_t framesInFlight = VulkanContext::GetVulkanSwapchain()->GetImageCount();

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.size = m_LocalData.Size;

		VulkanAllocator allocator("UniformBuffer");

		m_Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, m_Buffer);
	}
	void VulkanUniformBuffer::SetCurrentReadPointer(uint32_t writes, uint32_t size)
	{
		m_Writes = writes;
		m_Size = size;
		m_Offset = writes * size;
	}
	void VulkanUniformBuffer::ResizeBuffer(uint32_t newSize)
	{
		uint8_t* oldData = new uint8_t[m_LocalData.Size];
		uint32_t oldSize = m_LocalData.Size;
		memcpy_s(oldData, oldSize, m_LocalData.Data, oldSize);

		m_LocalData.Release();
		m_LocalData.Allocate(newSize);
		m_LocalData.Write(oldData, oldSize);

		delete[] oldData;
	}
}
#endif
