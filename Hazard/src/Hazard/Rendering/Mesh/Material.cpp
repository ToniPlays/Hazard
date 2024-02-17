#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	Material::Material(Ref<HazardRenderer::Pipeline> pipeline) : m_Pipeline(pipeline)
	{
		m_Pipeline = pipeline;
		Invalidate();
	}

	Material::~Material()
	{
		m_MaterialParams.clear();
		m_PushConstants.Release();
	}

	void Material::SetPipeline(Ref<HazardRenderer::Pipeline> pipeline)
	{
		if (pipeline == m_Pipeline) return;

		m_Pipeline = pipeline;
		Invalidate();
	}

	void Material::Set(const std::string& name, Ref<HazardRenderer::Cubemap> cubemap)
	{
		auto& spec = m_Pipeline->GetSpecifications();
		if (spec.SetLayouts.size() <= 1) return;

		for (auto& binding : spec.SetLayouts[1])
		{
			if (binding.Name != name) continue;
			m_DescriptorSet->Write(binding.Binding, 0, cubemap.As<Image>(), RenderEngine::GetResources().DefaultImageSampler, true);
		}
	}

	void Material::Set(const std::string& name, Ref<HazardRenderer::Image2D> image)
	{
		auto& spec = m_Pipeline->GetSpecifications();
		if (spec.SetLayouts.size() <= 1) return;

		for (auto& binding : spec.SetLayouts[1])
		{
			if (binding.Name != name) continue;
			m_DescriptorSet->Write(binding.Binding, 0, image.As<Image>(), RenderEngine::GetResources().DefaultImageSampler, true);
		}
	}

	void Material::Invalidate()
	{
		//Descriptor set 0 reserved for world info
		auto& spec = m_Pipeline->GetSpecifications();
		if (spec.SetLayouts.size() > 1)
		{
			auto layout = spec.SetLayouts[1];
			DescriptorSetCreateInfo setInfo = {
				.DebugName = fmt::format("Material: {}", m_Pipeline->GetSpecifications().DebugName),
				.Set = 1,
				.pLayout = &layout,
			};

			m_DescriptorSet = DescriptorSet::Create(&setInfo);
		}

		InvalidatePushConstants();
	}

	void Material::InvalidatePushConstants()
	{
		m_PushConstants.Release();
		m_MaterialParams.clear();

		auto& spec = m_Pipeline->GetSpecifications();

		uint32_t requiredSize = 0;
		for (auto& range : spec.PushConstants)
		{
			auto& param = m_MaterialParams[range.Name];
			param.Name = range.Name;
			param.Type = range.Type;
			param.Offset = range.Offset;

			requiredSize += ShaderDataTypeSize(range.Type);
		}
		m_PushConstants.Allocate(requiredSize);
		m_PushConstants.ZeroInitialize();
	}
}
