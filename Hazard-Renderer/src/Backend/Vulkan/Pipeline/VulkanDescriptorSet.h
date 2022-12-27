#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "vulkan/vulkan.h"

#include <unordered_map>
#include <vector>
#include <iostream>

namespace HazardRenderer::Vulkan
{
	struct WriteDescriptorData
	{
		std::string Name;
		VkWriteDescriptorSet Set;
		VkDescriptorType Type;
	};


	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet() = default;

		void SetDebugName(const std::string& name) { m_DebugName = name; }
		VkDescriptorSet& GetVulkanDescriptorSet() { return m_VkDescriptorSet; }
		VkDescriptorSetLayout& GetLayout() { return m_DescriptorSetLayout; }

		void SetBuffer(uint32_t binding, VkDescriptorBufferInfo info);
		void SetSampler(uint32_t binding, uint32_t index, VkDescriptorImageInfo info);
		void SetAccelerationStructure(uint32_t binding, uint32_t index, VkWriteDescriptorSetAccelerationStructureKHR info);

		void ReserveBindings(size_t size)
		{
			m_Bindings.reserve(size);
		}
		void AddBinding(VkDescriptorSetLayoutBinding binding)
		{
			m_Bindings.push_back(binding);
		}
		void AddWriteDescriptor(uint32_t binding, const std::string& name, VkWriteDescriptorSet descriptor)
		{
			m_WriteDescriptors[binding] = { name, descriptor };
			m_WriteDescriptorNames[name] = binding;
		}
		bool Contains(const std::string& name)
		{
			return m_WriteDescriptorNames.find(name) != m_WriteDescriptorNames.end();
		}
		uint32_t GetIndex(const std::string& name)
		{
			return m_WriteDescriptorNames[name];
		}

		void Invalidate();

	private:
		VkDescriptorSet m_VkDescriptorSet;
		std::unordered_map<uint32_t, WriteDescriptorData> m_WriteDescriptors;
		std::unordered_map<std::string, uint32_t> m_WriteDescriptorNames;
		std::vector<VkDescriptorSetLayoutBinding> m_Bindings;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::string m_DebugName;
	};
}
#endif
