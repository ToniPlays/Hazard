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
		~RenderContext();

		void Render() override;
		void PostRender() override;
		void Close() override;

		RenderAPI GetCurrentAPI() { return m_CurrentAPI; }
		GraphicsContext& GetContext() { return *m_Window->GetContext(); }
		Window& GetWindow() { return *m_Window; }
		uint32_t GetImagesInFlight() { return m_ImagesInFlight; }
		CommandQueue& GetRenderCommandQueue() { return *m_RenderCommadQueue; }
		CommandQueue& GetResourceCreateCommandQueue() { return *m_ResourceCreateCommadQueue; }
		CommandQueue& GetResourceFreeCommandQueue() { return *m_ResourceFreeCommadQueue; }

	public:
		static const char* APIToString(RenderAPI api);
		static RenderAPI StringToApi(const std::string& api);

	private:
		void Process(Event& e);
		void SetupQueues();

	private:
		RenderAPI m_CurrentAPI;
		Window* m_Window = nullptr;
		Color m_ClearColor;
		uint32_t m_ImagesInFlight;

		CommandQueue* m_RenderCommadQueue;
		CommandQueue* m_ResourceCreateCommadQueue;
		CommandQueue* m_ResourceFreeCommadQueue;
	};
}
