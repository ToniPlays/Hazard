#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Sampler.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanSampler : public Sampler
	{
	public:
		VulkanSampler(SamplerCreateInfo* createInfo);
		~VulkanSampler();

		const std::string& GetDebugName() const { return m_Info.DebugName; };
		FilterMode GetMinFilter() const { return m_Info.MinFilter; };
		FilterMode GetMagFilter() const { return m_Info.MagFilter; };
		ImageWrap GetWrapping() const { return m_Info.Wrapping; };
		//Vulkan specific

		void Invalidate();
		void Invalidate_RT();

		VkSampler GetVulkanSampler() const { return m_Sampler; }

	private:
		SamplerCreateInfo m_Info;
		VkSampler m_Sampler = VK_NULL_HANDLE;
	};
}
#endif