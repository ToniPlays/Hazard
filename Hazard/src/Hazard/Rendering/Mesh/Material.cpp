#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	Material::Material(Ref<HazardRenderer::Pipeline> pipeline) : m_Pipeline(pipeline)
	{
		m_Pipeline = pipeline;

		//Descriptor set 0 reserved for world info

		auto& spec = m_Pipeline->GetSpecifications();
		if (spec.SetLayouts.size() <= 1) return;

		auto layout = spec.SetLayouts[1];

		DescriptorSetCreateInfo setInfo = {
			.DebugName = fmt::format("Material: {}", pipeline->GetSpecifications().DebugName),
			.Set = 1,
			.pLayout = &layout,
		};

		m_DescriptorSet = DescriptorSet::Create(&setInfo);
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
}
