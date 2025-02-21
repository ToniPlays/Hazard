#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"
#include <Hazard/RenderContext/ShaderAsset.h>

namespace Hazard
{
	Material::Material(AssetHandle handle) : m_PipelineHandle(handle)
	{
		Invalidate();
	}

	Material::~Material()
	{
		m_MaterialParams.clear();
		m_PushConstants.Release();
	}

	void Material::SetPipeline(AssetHandle handle)
	{
		if (m_PipelineHandle == handle) return;
		m_PipelineHandle = handle;

		Invalidate();
	}

	bool Material::Set(const std::string& name, Ref<HazardRenderer::Cubemap> cubemap)
	{
		if (!m_DescriptorSet) return false;

		for (auto& [textureName, texture] : m_TextureParams)
		{
			if (textureName != name) continue;

			m_TextureParams[textureName].Value = cubemap;
			m_DescriptorSet->Write(texture.Binding, 0, cubemap.As<Cubemap>(), RenderContextManager::GetDefaultSampler(), true);
			return true;
		}
		return false;
	}

	bool Material::Set(const std::string& name, Ref<HazardRenderer::Image2D> image)
	{
		if (!m_DescriptorSet) return false;

		for (auto& [textureName, texture] : m_TextureParams)
		{
			if (textureName != name) continue;

			m_TextureParams[textureName].Value = image;
			m_DescriptorSet->Write(texture.Binding, 0, image.As<Image>(), RenderContextManager::GetDefaultSampler(), true);
			return true;
		}
		return false;
	}

	void Material::Invalidate()
	{
		if (m_PipelineHandle == INVALID_ASSET_HANDLE) return;

		Ref<ShaderAsset> asset = AssetManager::GetAsset<ShaderAsset>(m_PipelineHandle);

		//Descriptor set 0 reserved for world info
		auto& spec = asset->GetPipeline()->GetSpecifications();
		if (spec.SetLayouts.size() > 1)
		{
			auto layout = spec.SetLayouts[1];
			DescriptorSetCreateInfo setInfo = {
				.DebugName = fmt::format("Material: {}", spec.DebugName),
				.Set = 1,
				.pLayout = &layout,
			};

			m_DescriptorSet = DescriptorSet::Create(&setInfo);

			InvalidateDescriptorSet();
		}

		InvalidatePushConstants();
	}

	void Material::InvalidatePushConstants()
	{
		m_PushConstants.Release();
		m_MaterialParams.clear();

		/*auto& spec = m_Pipeline->GetSpecifications();

		uint32_t requiredSize = 0;
		for (auto& range : spec.PushConstants)
		{
			auto& param = m_MaterialParams[range.Name];
			param.Name = range.Name;
			param.Type = range.Type;
			param.Offset = range.Offset;

			requiredSize += std::max<uint32_t>(4, ShaderDataTypeSize(range.Type));
		}
		m_PushConstants.Allocate(requiredSize);
		m_PushConstants.ZeroInitialize();
		*/
	}
	void Material::InvalidateDescriptorSet()
	{
		/*
		m_TextureParams.clear();
		if (!m_DescriptorSet) return;

		const DescriptorSetLayout& layout = m_DescriptorSet->GetLayout();

		for (auto& param : layout.GetElements())
		{
			if (param.Type & ~(DESCRIPTOR_TYPE_SAMPLER_2D, DESCRIPTOR_TYPE_SAMPLER_CUBE)) continue;

			m_TextureParams[param.Name] = TextureParam {
				.Name = param.Name,
				.Binding = param.Binding,
			};
		}
		*/
	}
}
