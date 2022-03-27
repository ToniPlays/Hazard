
#include "VulkanBuffers.h"
#ifdef HZR_INCLUDE_VULKAN
#include "../VulkanContext.h"
#include "../VulkanRenderCommandBuffer.h"


using namespace HazardUtility;

namespace HazardRenderer::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(VertexBufferCreateInfo* info) : m_Size(info->Size)
	{
		ASSERT(info->Layout != nullptr, "Buffer layout cannot be nullptr");

		m_DebugName = info->DebugName;
		m_Usage = info->Usage;

		m_Layout = std::move(*info->Layout);
		VulkanDevice& device = VulkanContext::GetPhysicalDevice();

		VulkanAllocator allocator("VertexBuffer");

		if (info->Data == nullptr) {

			VkBufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.size = info->Size;
			createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			m_Allocation = allocator.AllocateBuffer(createInfo, VMA_MEMORY_USAGE_GPU_TO_CPU, m_Buffer);
			m_LocalData.Allocate(m_Size);
		}
		else
		{
			m_LocalData = Buffer::Copy(info->Data, info->Size);

			VkBufferCreateInfo stagingCreateInfo = {};
			stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingCreateInfo.size = info->Size;
			stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);
			memcpy_s(destData, info->Size, m_LocalData.Data, m_LocalData.Size);
			allocator.UnmapMemory(stagingBufferAlloc);

			VkBufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.size = info->Size;
			createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			m_Allocation = allocator.AllocateBuffer(createInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Buffer);

			VkCommandBuffer copyCmd = device.GetCommandBuffer(true);
			VkBufferCopy copyRegion = {};
			copyRegion.size = m_LocalData.Size;

			vkCmdCopyBuffer(copyCmd, stagingBuffer, m_Buffer, 1, &copyRegion);
			device.FlushCommandBuffer(copyCmd);
			allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);

		}
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		m_LocalData.Release();

		VulkanAllocator allocator("VertexBuffer");
		allocator.DestroyBuffer(m_Buffer, m_Allocation);
	}
	void VulkanVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		ASSERT(cmdBuffer->IsRecording(), "CommandBuffer not in recording state");
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		auto vkCmdBuffer = cmdBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(vkCmdBuffer, 0, 1, &m_Buffer, offsets);
	}
	void VulkanVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		m_LocalData.Write(data, size, 0);
		RT_SetData(data, size);
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
		if (info->Data == nullptr) return;
		SetData(info->Data, info->Size);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{

		VulkanAllocator allocator("IndexBuffer");
		allocator.DestroyBuffer(m_Buffer, m_Allocation);
	}
	void VulkanIndexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		auto vkCmdBuffer = cmdBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindIndexBuffer(vkCmdBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	void VulkanIndexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void VulkanIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		m_Size = size;
		VulkanDevice& device = VulkanContext::GetPhysicalDevice();
		VulkanAllocator allocator("IndexBuffer");
		m_LocalData = Buffer::Copy(data, m_Size * sizeof(uint32_t));

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_LocalData.Size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		//Copy data to staging buffer
		uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
		memcpy_s(destData, m_LocalData.Size, m_LocalData.Data, m_LocalData.Size);
		allocator.UnmapMemory(stagingBufferAllocation);

		VkBufferCreateInfo indexBufferCreateInfo = {};
		indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferCreateInfo.size = m_LocalData.Size;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		m_Allocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Buffer);

		VkCommandBuffer copyCmd = device.GetCommandBuffer(true);
		VkBufferCopy copyRegion = {};
		copyRegion.size = m_LocalData.Size;
		vkCmdCopyBuffer(
			copyCmd,
			stagingBuffer,
			m_Buffer,
			1,
			&copyRegion);

		device.FlushCommandBuffer(copyCmd);

		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}
	VulkanUniformBuffer::VulkanUniformBuffer(UniformBufferCreateInfo* createInfo) : m_Size(createInfo->Size),
		m_Binding(createInfo->Binding), m_Usage(createInfo->Usage)
	{

		ASSERT(m_Size >= 256, "Need a bigger buffer");
		m_Name = createInfo->Name;
		m_LocalData.Allocate(m_Size * 32);
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
	void VulkanUniformBuffer::Unbind()
	{

	}
	void VulkanUniformBuffer::SetData(const void* data, uint32_t size)
	{
		/*if (m_Writes * m_Size > m_LocalData.Size) {
			ResizeBuffer(m_Writes * m_Size);
		}
		m_LocalData.Write(data, size, m_Writes * m_Size);

		Ref<VulkanUniformBuffer> instance = this;

		RenderContextCommand::SubmitResourceCreate([instance]() mutable {
			instance->RT_SetData();
			});
		RenderContextCommand::Submit([instance, writes = m_Writes, size = m_Size]() mutable {
			HZR_PROFILE_FUNCTION("VulkanUniformBuffer::SetCurrentReadPointer() RT");
			instance->SetCurrentReadPointer(writes, size);
			});

		if (m_Writes > 0) {
			m_Offset += m_Size;
		}
		m_Writes++;*/
	}
	void VulkanUniformBuffer::RT_SetData()
	{
		/*HZR_PROFILE_FUNCTION("Hazard::Rendering::VulkanUniformBuffer::RT_SetData() RT");
		VulkanAllocator allocator("VulkanUniformBuffer");

		uint8_t* pData = allocator.MapMemory<uint8_t>(m_Allocation);
		memcpy_s(pData, m_LocalData.Size, (const uint8_t*)m_LocalData.Data, m_LocalData.Size);
		allocator.UnmapMemory(m_Allocation);*/

	}
	void VulkanUniformBuffer::Release()
	{
		/*RenderContextCommand::SubmitResourceFree([buffer = m_Buffer, alloc = m_Allocation]() {
			VulkanAllocator allocator("UniformBuffer");
			allocator.DestroyBuffer(buffer, alloc);
			});*/

		m_Buffer = nullptr;
		m_Allocation = nullptr;
		m_LocalData.Release();
	}
	void VulkanUniformBuffer::RT_Invalidate()
	{
		if (m_Allocation)
			Release();

		/*auto device = VulkanContext::GetDevice()->GetDevice();
		uint32_t framesInFlight = RenderContextCommand::GetImagesInFlight();

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

		m_Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, m_Buffer);*/
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
