#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	Material::Material() {}

	Material::Material(Ref<HazardRenderer::Pipeline> pipeline, HazardRenderer::DescriptorSetLayout layout) : m_Pipeline(pipeline)
	{
		if (layout.GetElementCount() == 0) return;

		DescriptorSetCreateInfo setInfo = {
			.DebugName = fmt::format("Material: {}", pipeline->GetSpecifications().DebugName),
			.Set = 1,
			.pLayout = &layout,
		};

		m_DescriptorSet = DescriptorSet::Create(&setInfo);
	}
}
