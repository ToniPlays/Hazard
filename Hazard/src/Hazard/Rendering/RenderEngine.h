#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"

namespace Hazard::Rendering {

	class Renderer2D;

	class RenderEngine : public Module::Module
	{
	public:
		RenderEngine(RenderEngineCreateInfo* info, RenderAPI api);
		~RenderEngine() = default;

		void Update() override;

	private:
		Renderer2D* m_Renderer2D;
	};
}