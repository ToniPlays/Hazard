#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Core/Rendering/Cubemap.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		DescriptorSetLayout skyboxMaterialLayout = {
				{ SHADER_STAGE_VERTEX_BIT, "u_Camera", 0, DESCRIPTOR_TYPE_UNIFORM_BUFFER },
				{ SHADER_STAGE_FRAGMENT_BIT, "u_CubeMap", 1, DESCRIPTOR_TYPE_SAMPLER_CUBE }
		};

		DescriptorSetCreateInfo skyboxSetSpec = {
			.DebugName = "Skybox descriptor",
			.Set = 0,
			.pLayout = &skyboxMaterialLayout,
		};

		m_DescriptorSet = DescriptorSet::Create(&skyboxSetSpec);
	}
}
