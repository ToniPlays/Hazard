#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include "Hazard/Core/ApplicationCreateInfo.h"

namespace Hazard::Rendering {

	class RenderContext : public Module::Module {

	public:
		RenderContext(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo);
		~RenderContext() = default;

		void PreRender() override;
		void PostRender() override;
		void Close() override;

		RenderAPI GetCurrentAPI() { return m_CurrentAPI; }
		GraphicsContext& GetContext() { return *m_Window->GetContext(); }
		Window& GetWindow() { return *m_Window; }
		uint32_t GetImagesInFlight() { return m_ImagesInFlight; }

	public:
		static const char* APIToString(RenderAPI api);
		static RenderAPI StringToApi(const std::string& api);

	private:
		void Process(Event& e);

	private:
		RenderAPI m_CurrentAPI;
		Window* m_Window = nullptr;
		Color m_ClearColor;
		uint32_t m_ImagesInFlight;
	};
}
