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
		void Close() override;

		HazardRenderer::Window& GetWindow() { return *m_Window; }
		HazardRenderer::DefaultResources& GetDefaultResources() { return m_Window->GetContext()->GetDefaultResources(); }

	private:
		HazardRenderer::Window* m_Window;

	};
}