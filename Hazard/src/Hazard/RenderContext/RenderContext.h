#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include "Hazard/Core/ApplicationCreateInfo.h"

namespace Hazard::Rendering {

	class RenderContext : public Module::Module {

	public:
		RenderContext();
		~RenderContext();

		void InitContext(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo);
		void Update() override;
		void Close();

		void SetClearColor(Color color) { m_ClearColor = color; };

		RenderAPI GetCurrentAPI() { return m_CurrentAPI; }
		GraphicsContext& GetContext() { return *m_Window->GetContext(); }
		Window& GetWindow() { return *m_Window; }

		static const char* APIToString(RenderAPI api);

	private:
		void Process(Event& e);

	private:
		RenderAPI m_CurrentAPI;
		Window* m_Window = nullptr;
		Color m_ClearColor;

	};
}