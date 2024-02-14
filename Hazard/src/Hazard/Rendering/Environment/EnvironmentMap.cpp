#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		Ref<Pipeline> skyboxPipeline = ShaderLibrary::GetPipeline("Skybox");
		m_Material = Ref<Material>::Create(skyboxPipeline);
	}

	void EnvironmentMap::Invalidate()
	{
		m_Material->Set("u_CubeMap", m_EnvironmentMap);
	}
}
