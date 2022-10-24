#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

namespace Hazard
{
	class EnvironmentMap : public Asset 
	{
	public:
		EnvironmentMap() = default;

		Ref<HazardRenderer::CubemapTexture> RadianceMap = nullptr;
		Ref<HazardRenderer::CubemapTexture> IrradianceMap = nullptr;
		Ref<HazardRenderer::CubemapTexture> PreFilterMap = nullptr;
		Ref<HazardRenderer::Image2D> BRDFLut = nullptr;
	};
}