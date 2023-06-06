
#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_VULKAN
#include "Backend/Core/Buffers.h"

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace HazardRenderer::Vulkan
{
	class VulkanArgumentBuffer : public ArgumentBuffer
	{
	public:
		VulkanArgumentBuffer(ArgumentBufferCreateInfo* createInfo);
		~VulkanArgumentBuffer();

		const std::string& GetDebugName() override { return m_DebugName; }

		void SetData(const BufferCopyRegion& copyRegion) override;
		const size_t GetSize() const override { return m_Size; }

		//Vulkan specific
		void Invalidate();
		void Invalidate_RT();
		void SetData_RT(const BufferCopyRegion& copyRegion);

		VkBuffer GetVulkanBuffer() const { return m_VulkanBuffer; }

	private:
		std::string m_DebugName;
		size_t m_Size;
		Buffer m_LocalData;

		VkBuffer m_VulkanBuffer;
		VmaAllocation m_BufferAllocation;
	};
}
#endif