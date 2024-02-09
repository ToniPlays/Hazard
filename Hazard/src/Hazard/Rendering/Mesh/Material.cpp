#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	Material::Material() {}

	Material::Material(Ref<HazardRenderer::Pipeline> pipeline, HazardRenderer::DescriptorSetLayout layout) : m_Pipeline(pipeline)
	{
		if (layout.GetElementCount() == 0) return;

		DescriptorSetCreateInfo setInfo = {};
		setInfo.DebugName = "Material " + pipeline->GetSpecifications().DebugName;
		setInfo.pLayout = &layout;
		setInfo.Set = 1;

		m_DescriptorSet = DescriptorSet::Create(&setInfo);
	}
}
