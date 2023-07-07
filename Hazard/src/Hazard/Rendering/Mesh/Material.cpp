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
		DescriptorSetLayout layout = {
			{ "Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER }
		};

		DescriptorSetCreateInfo setInfo = {};
		setInfo.DebugName = "MaterialDescriptorSet 0";
		setInfo.Set = 0;
		setInfo.pLayout = &layout;
		m_DescriptorSet = DescriptorSet::Create(&setInfo);

		m_DescriptorSet->Write(0, RenderEngine::GetResources().CameraUniformBuffer, true);
	}
}
