#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "HazardRenderer.h"

namespace Hazard 
{
	class RenderContextManager : public Module 
	{
	public:
		RenderContextManager(ApplicationCreateInfo* appInfo, RenderContextCreateInfo* createInfo);
		~RenderContextManager() = default;

		void PreRender() override;
		void PostRender() override;

		HazardRenderer::Window& GetWindow() { return *m_Window; }

	private:
		HazardRenderer::Window* m_Window;

	};
}