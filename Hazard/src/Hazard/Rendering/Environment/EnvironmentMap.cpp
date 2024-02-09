#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Core/Rendering/Texture.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		m_Spec.Resolution = 2048;
		m_Spec.Samples = 128;
	}

	void EnvironmentMap::Update(uint32_t samples, uint32_t resolution, AssetHandle sourceImage)
	{
		m_SourceImageHandle = sourceImage;
		m_Spec.Samples = samples;
		m_Spec.Resolution = resolution;
	}

	void EnvironmentMap::Invalidate() 
	{
		if (m_SourceImageHandle != INVALID_ASSET_HANDLE)
			GenerateRadiance();
	}

	void EnvironmentMap::GenerateRadiance()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
	}

	void EnvironmentMap::GenerateIrradiance(Ref<AssetPointer> radianceMap)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
	}

	void EnvironmentMap::GeneratePreFilter(Ref<AssetPointer> radiance)
	{
		HZR_PROFILE_FUNCTION();
	}
}
