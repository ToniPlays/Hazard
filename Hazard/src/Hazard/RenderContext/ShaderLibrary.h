#pragma once

#include "Backend/Core/Pipeline/Pipeline.h"

namespace Hazard
{
	class ShaderLibrary
	{
	public:
		static Ref<HazardRenderer::Pipeline> GetPipeline(const std::string& name);
	};
}
