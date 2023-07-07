#include <hzrpch.h>
#include "Material.h"

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	Material::Material()
	{
		SetPipeline(AssetManager::GetHandleFromKey("PBR_Static.glsl.hpack"));
	}
	Material::Material(AssetHandle pipelineHandle) : m_PipelineHandle(pipelineHandle)
	{
		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(pipelineHandle)->Value.As<Pipeline>();
		DescriptorSetLayout layout = pipeline->GetDescriptorSetLayout(0);

		DescriptorSetCreateInfo setInfo = {};
		setInfo.DebugName = fmt::format("Material {}", pipeline->GetSpecifications().DebugName);
		setInfo.Set = 0;
		setInfo.pLayout = &layout;
		m_DescriptorSet = DescriptorSet::Create(&setInfo);

		m_DescriptorSet->Write(0, RenderEngine::GetResources().CameraUniformBuffer, true);
	}
}
