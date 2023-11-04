#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	Material::Material() {}

	Material::Material(AssetHandle pipelineHandle, HazardRenderer::DescriptorSetLayout layout) : m_PipelineHandle(pipelineHandle)
	{
		if (layout.GetElementCount() == 0) return;
		
		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(pipelineHandle)->Value.As<Pipeline>();

		DescriptorSetCreateInfo setInfo = {};
		setInfo.DebugName = "Material " + pipeline->GetSpecifications().DebugName;
		setInfo.pLayout = &layout;
		setInfo.Set = 1;

		m_DescriptorSet = DescriptorSet::Create(&setInfo);
	}
}
