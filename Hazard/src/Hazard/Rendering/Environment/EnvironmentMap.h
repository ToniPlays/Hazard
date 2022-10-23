#pragma once

#include "Hazard/Assets/Asset.h"

namespace Hazard
{
	class EnvironmentMap : public Asset 
	{
	public:
		EnvironmentMap() = default;

		Ref< HazardRenderer::CubemapTexture> RadianceMap;
		Ref< HazardRenderer::CubemapTexture> IrradianceMap;
		Ref< HazardRenderer::CubemapTexture> PreFilterMap;
		Ref< HazardRenderer::Image2D> BRDFLut;
	};
}