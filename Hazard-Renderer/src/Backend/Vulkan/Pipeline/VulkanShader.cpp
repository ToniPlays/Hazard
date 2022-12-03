
#include "VulkanShader.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"

#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "VulkanUniformBuffer.h"
#include "../Textures/VulkanImage2D.h"
#include "../Textures/VulkanCubemapTexture.h"
#include "../AccelerationStructure/VulkanTopLevelAS.h"
#include "Profiling/Timer.h"

#include "Backend/Core/ShaderCompiler.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanShader::VulkanShader(const std::vector<ShaderStageCode>& shaderCode)
	{
		HZR_PROFILE_FUNCTION();

		for (auto& code : shaderCode)
			m_ShaderCode[code.Stage] = Buffer::Copy(code.ShaderCode);

		Reload();
	}
	VulkanShader::~VulkanShader()
	{
		HZR_PROFILE_FUNCTION();
		Renderer::SubmitResourceFree([modules = m_ShaderModules]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			for (auto& shader : modules)
				vkDestroyShaderModule(device, shader, nullptr);
			});
	}
	void VulkanShader::Reload()
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;

		Reflect();

		Ref<VulkanShader> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->CreateShaderModules();
			instance->CreateDescriptorSetLayouts();
			});
	}
	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<Image2D> image)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanShader> instance = this;
		Ref<VulkanImage2D> vulkanImage = image.As<VulkanImage2D>();

		Renderer::Submit([instance, vulkanImage, index, name]() mutable {
			uint32_t frameIndex = VulkanContext::GetFrameIndex();
			for (auto& set : instance->m_DescriptorSets[frameIndex])
			{
				if (!set.Contains(name)) continue;
				uint32_t binding = set.GetIndex(name);
				set.SetSampler(binding, index, vulkanImage->GetImageDescriptor());
				return;
			}
			});
	}
	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanShader> instance = this;
		Ref<VulkanCubemapTexture> vulkanCubemap = cubemap.As<VulkanCubemapTexture>();
		Renderer::Submit([instance, vulkanCubemap, index, name]() mutable {
			uint32_t frameIndex = VulkanContext::GetFrameIndex();

			for (auto& set : instance->m_DescriptorSets[frameIndex])
			{
				if (!set.Contains(name)) continue;

				uint32_t binding = set.GetIndex(name);
				set.SetSampler(set.GetIndex(name), index, vulkanCubemap->GetImageDescriptor());
				return;
			}
			});
	}

	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<AccelerationStructure> accelerationStructure)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanShader> instance = this;
		Renderer::Submit([instance, accelerationStructure, index, name]() mutable {
			uint32_t frameIndex = VulkanContext::GetFrameIndex();

			for (auto& set : instance->m_DescriptorSets[frameIndex])
			{
				if (!set.Contains(name)) continue;

				VkAccelerationStructureKHR s = {};
				//if(accelerationStructure->GetLevel() == AccelerationStructureLevel::Top)
					s = accelerationStructure.As<VulkanTopLevelAS>()->GetVulkanAccelerationStructure().AccelerationStructure;

				VkWriteDescriptorSetAccelerationStructureKHR info = {};
				info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
				info.accelerationStructureCount = 1;
				info.pAccelerationStructures = &s;

				uint32_t binding = set.GetIndex(name);
				set.SetAccelerationStructure(set.GetIndex(name), index, info);
				return;
			}
			});
	}

	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<BufferBase> buffer)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanShader> instance = this;
		Ref<BufferBase> b = buffer.As<BufferBase>();
		Renderer::Submit([instance, b, index, name]() mutable {
			uint32_t frameIndex = VulkanContext::GetFrameIndex();

			for (auto& set : instance->m_DescriptorSets[frameIndex])
			{
				if (!set.Contains(name)) continue;
				uint32_t binding = set.GetIndex(name);
				
				switch (b->GetType())
				{
				case BufferType::Vertex:
				{
					auto buf = b.As<VulkanVertexBuffer>();

					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = buf->GetVulkanBuffer();
					bufferInfo.offset = 0;
					bufferInfo.range = VK_WHOLE_SIZE;

					set.SetBuffer(binding, bufferInfo);
					break;
				}
				case BufferType::Index:
				{
					auto buf = b.As<VulkanIndexBuffer>();

					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = buf->GetVulkanBuffer();
					bufferInfo.offset = 0;
					bufferInfo.range = VK_WHOLE_SIZE;

					set.SetBuffer(binding, bufferInfo);
					break;
				}
					default: HZR_ASSERT(false, "TODO");
				}

				return;
			}
			});
	}

	void VulkanShader::Reload_RT(bool forceCompile)
	{
		HZR_PROFILE_FUNCTION();
	}
	std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts()
	{
		HZR_PROFILE_FUNCTION();
		uint32_t frameIndex = VulkanContext::GetFrameIndex();
		std::vector<VkDescriptorSetLayout> result(m_DescriptorSets[frameIndex].size());

		for (uint32_t i = 0; i < result.size(); i++)
			result[i] = m_DescriptorSets[frameIndex][i].GetLayout();

		return result;
	}
	VkDescriptorSetLayout& VulkanShader::GetDescriptorSetLayout(uint32_t index)
	{
		HZR_PROFILE_FUNCTION();
		uint32_t frameIndex = VulkanContext::GetFrameIndex();
		return m_DescriptorSets[frameIndex][index].GetLayout();
	}
	std::vector<VulkanDescriptorSet>& VulkanShader::GetDescriptorSets()
	{
		HZR_PROFILE_FUNCTION();
		uint32_t frameIndex = VulkanContext::GetFrameIndex();
		return m_DescriptorSets[frameIndex];
	}
	std::vector<VkDescriptorSet> VulkanShader::GetVulkanDescriptorSets()
	{
		HZR_PROFILE_FUNCTION();
		uint32_t frameIndex = VulkanContext::GetFrameIndex();
		std::vector<VkDescriptorSet> result(m_DescriptorSets[frameIndex].size());

		for (uint32_t i = 0; i < result.size(); i++)
			result[i] = m_DescriptorSets[frameIndex][i].GetVulkanDescriptorSet();

		return result;
	}
	const std::vector<uint32_t>& VulkanShader::GetDynamicOffsets()
	{
		HZR_PROFILE_FUNCTION();
		uint32_t i = 0;
		for (auto& [set, buffers] : m_UniformBuffers)
		{
			for (auto& [binding, uniformBuffer] : buffers)
			{
				m_DynamicOffsets[i] = uniformBuffer.As<VulkanUniformBuffer>()->GetOffset();
				i++;
			}
		}
		return m_DynamicOffsets;
	}
	void VulkanShader::Reflect()
	{
		HZR_PROFILE_FUNCTION();

		Timer timer;

		m_ShaderData.Stages.clear();
		m_ShaderData = ShaderCompiler::GetShaderResources(m_ShaderCode);

		uint32_t descriptorSets = 0;
		uint32_t size = 0;
		for (auto& [set, buffers] : m_ShaderData.UniformsDescriptions)
		{
			for (auto& [binding, buffer] : buffers)
			{
				UniformBufferCreateInfo info = {};
				info.Name = buffer.Name;
				info.Set = set;
				info.Binding = binding;
				info.Size = buffer.Size;
				info.Usage = buffer.UsageFlags;

				m_UniformBuffers[set][binding] = UniformBuffer::Create(&info);
				size++;
			}
			if (set > descriptorSets)
				descriptorSets = set;
		}
		for (auto& [set, buffer] : m_ShaderData.ImageSamplers)
		{
			if (set > descriptorSets)
				descriptorSets = set;
		}
		m_DynamicOffsets.resize(size);
		m_DescriptorSets.resize(VulkanContext::GetImagesInFlight());

		for (uint32_t i = 0; i < VulkanContext::GetImagesInFlight(); i++)
			m_DescriptorSets[i].resize(descriptorSets + 1);

		std::cout << fmt::format("Shader reflection took {0}", timer.ElapsedMillis()) << std::endl;
	}
	void VulkanShader::CreateShaderModules()
	{
		HZR_PROFILE_FUNCTION();
		const auto& device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		uint32_t index = 0;

		m_ShaderModules.resize(m_ShaderCode.size());

		for (auto& [stage, code] : m_ShaderCode) {

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.Size;
			createInfo.pCode = (uint32_t*)code.Data;

			VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, &m_ShaderModules[index]), "Failed to create VkShaderModule");

			auto& stageInfo = m_ShaderStageCreateInfos.emplace_back();
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.module = m_ShaderModules[index];
			stageInfo.pName = "main";
			stageInfo.flags = 0;
			stageInfo.stage = (VkShaderStageFlagBits)VkUtils::GetVulkanShaderStage((uint32_t)stage);

			index++;
		}
	}
	void VulkanShader::CreateDescriptorSetLayouts()
	{
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		uint32_t imagesInFlight = VulkanContext::GetImagesInFlight();

		for (uint32_t frame = 0; frame < imagesInFlight; frame++)
		{
			for (uint32_t set = 0; set < m_DescriptorSets[frame].size(); set++)
			{
				auto& descriptorSet = m_DescriptorSets[frame][set];
				descriptorSet.ReserveBindings(m_ShaderData.UniformsDescriptions[set].size() + m_ShaderData.ImageSamplers[set].size());

				for (auto& [binding, accel] : m_ShaderData.AccelerationStructures[set])
				{
					VkDescriptorSetLayoutBinding descriptorBinding = {};
					descriptorBinding = {};
					descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
					descriptorBinding.binding = accel.Location;
					descriptorBinding.descriptorCount = 1;
					descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage((uint32_t)accel.UsageFlags);

					VkWriteDescriptorSet writeDescriptor = {};
					writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeDescriptor.dstBinding = accel.Location;
					writeDescriptor.descriptorType = descriptorBinding.descriptorType;
					writeDescriptor.dstArrayElement = 0;
					writeDescriptor.descriptorCount = 1;

					descriptorSet.AddBinding(descriptorBinding);
					descriptorSet.AddWriteDescriptor(accel.Location, accel.Name, writeDescriptor);
				}
				for (auto& [binding, buffer] : m_ShaderData.StorageBuffers[set])
				{
					VkDescriptorSetLayoutBinding descriptorBinding = {};
					descriptorBinding = {};
					descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					descriptorBinding.binding = buffer.Location;
					descriptorBinding.descriptorCount = 1;
					descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage((uint32_t)buffer.UsageFlags);

					VkWriteDescriptorSet writeDescriptor = {};
					writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeDescriptor.dstBinding = buffer.Location;
					writeDescriptor.descriptorType = descriptorBinding.descriptorType;
					writeDescriptor.dstArrayElement = 0;
					writeDescriptor.descriptorCount = 1;

					descriptorSet.AddBinding(descriptorBinding);
					descriptorSet.AddWriteDescriptor(buffer.Location, buffer.Name, writeDescriptor);
				}
				for (auto& [binding, buffer] : m_ShaderData.UniformsDescriptions[set])
				{
					VkDescriptorSetLayoutBinding descriptorBinding = {};
					descriptorBinding = {};
					descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					descriptorBinding.binding = binding;
					descriptorBinding.descriptorCount = 1;
					descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(buffer.UsageFlags);

					VkWriteDescriptorSet writeDescriptor = {};
					writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeDescriptor.dstBinding = binding;
					writeDescriptor.descriptorType = descriptorBinding.descriptorType;
					writeDescriptor.dstArrayElement = 0;
					writeDescriptor.descriptorCount = 1;

					descriptorSet.AddBinding(descriptorBinding);
					descriptorSet.AddWriteDescriptor(binding, buffer.Name, writeDescriptor);
				}
				for (auto& [binding, sampler] : m_ShaderData.ImageSamplers[set])
				{
					VkDescriptorSetLayoutBinding descriptorBinding = {};
					descriptorBinding = {};
					descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorBinding.binding = binding;
					descriptorBinding.descriptorCount = sampler.ArraySize;
					descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(sampler.Flags);

					VkWriteDescriptorSet writeDescriptor = {};
					writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeDescriptor.dstBinding = binding;
					writeDescriptor.descriptorType = descriptorBinding.descriptorType;
					writeDescriptor.descriptorCount = 1;

					descriptorSet.AddBinding(descriptorBinding);
					descriptorSet.AddWriteDescriptor(binding, sampler.Name, writeDescriptor);
				}
				for (auto& [binding, storageImage] : m_ShaderData.StorageImages[set])
				{
					VkDescriptorSetLayoutBinding descriptorBinding = {};
					descriptorBinding = {};
					descriptorBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
					descriptorBinding.binding = binding;
					descriptorBinding.descriptorCount = storageImage.ArraySize;
					descriptorBinding.stageFlags = VkUtils::GetVulkanShaderStage(storageImage.Flags);

					VkWriteDescriptorSet writeDescriptor = {};
					writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeDescriptor.dstBinding = binding;
					writeDescriptor.descriptorType = descriptorBinding.descriptorType;
					writeDescriptor.descriptorCount = 1;

					descriptorSet.AddBinding(descriptorBinding);
					descriptorSet.AddWriteDescriptor(binding, storageImage.Name, writeDescriptor);
				}

				descriptorSet.Invalidate();

				for (auto& [binding, buffer] : m_UniformBuffers[set])
				{
					auto vulkanBuffer = buffer.As<VulkanUniformBuffer>();

					VkDescriptorBufferInfo bufferInfo = {};
					bufferInfo.buffer = vulkanBuffer->GetVulkanBuffer();
					bufferInfo.offset = 0;
					bufferInfo.range = vulkanBuffer->GetSize();

					descriptorSet.SetBuffer(vulkanBuffer->GetBinding(), bufferInfo);
				}

				Ref<VulkanImage2D> whiteTexture = VulkanContext::GetInstance()->GetDefaultResources().WhiteTexture;
				for (auto& [binding, sampler] : m_ShaderData.ImageSamplers[set])
				{
					for (uint32_t i = 0; i < sampler.ArraySize; i++)
					{
						if (sampler.Dimension == 2)
							descriptorSet.SetSampler(binding, i, whiteTexture->GetImageDescriptor());
					}
				}
			}
		}
	}
	void VulkanShader::CreatePushConstantRanges()
	{
		HZR_PROFILE_FUNCTION();
	}
}
#endif
