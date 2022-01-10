#pragma once

#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Pipeline/Buffers.h"
#include "vulkan/vulkan.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanUniformBuffer;

	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& file);
		~VulkanShader();

		void Reload() override;
		void Bind(Ref<RenderCommandBuffer> cmdBufer) override;
		void Unbind() override;
		void SetUniformBuffer(const std::string& name, void* data) override;
		void Set(const std::string& name, uint32_t index, uint32_t value) override {};
		void Set(const std::string& name, uint32_t index, Ref<Texture2D>& value) override;

		UniformBuffer& GetUniform(const std::string& name) override { return *m_UniformBuffers[m_UnformBufferBindings[name]]; };

		VkDescriptorSetLayout GetDescriptorLayout() { return m_Layout; }
		std::unordered_map<uint32_t, Ref<UniformBuffer>> GetUniformBuffers() { return m_UniformBuffers; }

		VkVertexInputBindingDescription GetBindingDescriptions();
		std::vector<VkVertexInputAttributeDescription> GetAttriDescriptions();

		const ShaderData& GetShaderData() override { return m_ShaderData; };
		VkDescriptorSet* GetDescriptorSet() { return &m_DescriptorSets[0]; }
		std::vector<VkPipelineShaderStageCreateInfo> GetStageInfo();
		std::vector<uint32_t> GetDynamicOffsets();

		void DestroyModules();
		
	private:

		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		void CreateModules();
		void Reflect();
		VkResult CreateDescriptorLayout();
		VkResult CreateDescriptorSets();
		void UpdateBufferDescriptor(Ref<VulkanUniformBuffer> buffer, uint32_t index);

		std::string m_Path;
		ShaderData m_ShaderData;
		VkDescriptorSetLayout m_Layout;
		std::vector<VkDescriptorSet> m_DescriptorSets;
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_ShaderCode;
		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_Modules;

		std::unordered_map<std::string, VkWriteDescriptorSet> m_WriteDescriptors;
		std::unordered_map<uint32_t, Ref<UniformBuffer>> m_UniformBuffers;
		std::unordered_map<std::string, uint32_t> m_UnformBufferBindings;
		std::vector<uint32_t> m_DynamicOffsets;
	};
}