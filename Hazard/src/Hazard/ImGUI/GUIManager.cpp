#include "GUIManager.h"
#include "Hazard.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "../ImGui_Backend/FontAwesome.h"
#include "Platform/Vulkan/GUIPlatformVulkan.h"
#include "Platform/Metal/GUIPlatformMetal.h"

#include "imgui.h"
#include "../../../vendor/ImGui_Backend/ImGuizmo.h"


namespace Hazard 
{
	void GUIManager::Init()
	{
		m_Window = &Application::Get().GetModule<RenderContextManager>().GetWindow();
    
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;
		if(GraphicsContext::GetRenderAPI() == RenderAPI::OpenGL)
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 18.0f);
		io.DisplaySize = { (float)m_Window->GetWidth(), (float)m_Window->GetHeight() };

		ImFontConfig config;
		config.MergeMode = true;
		static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		io.Fonts->AddFontFromFileTTF("res/fonts/fontawesome-webfont.ttf", 18.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 32.0f);

		//Initialize style
		ImUI::StyleManager::LoadStyle(ImUI::Style());

		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		InitImGuiPlatform(*m_Window);
	}

	void GUIManager::Update()
	{
		HZR_PROFILE_FUNCTION();
		for (auto& [type, panels] : m_Renderables)
			for (auto& panel : panels)
				panel->Update();

	}
	void GUIManager::Render()
	{
		Renderer::Submit([this]() mutable {
			m_Platform->BeginFrame();
			ImGuizmo::BeginFrame();

			if (m_Menubar)
				m_Menubar->Render();

			{
				using namespace ImUI;
				ScopedStyleVar style(ImGuiStyleVar_FramePadding, ImVec2(16.0f, 8.0f));
				Dockspace::BeginDockspace("MainWorkspace", ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_PassthruCentralNode);
				Dockspace::EndDockspace("MainWorkspace");
			}

			for (auto& [type, panels] : m_Renderables)
			{
				for (auto& panel : panels)
					panel->Render();
			}

			ImGui::Render();
			m_Platform->EndFrame();
		});
	}
	bool GUIManager::OnEvent(Event& e)
	{
		for (auto& [type, panels] : m_Renderables)
		{
			for (auto& panel : panels)
				if (panel->OnEvent(e)) return true;
		}

		if (m_Menubar)
		{
			if (m_Menubar->OnEvent(e)) 
				return true;
		}

		return false;
	}
	void GUIManager::InitImGuiPlatform(HazardRenderer::Window& window)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
	#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:
			m_Platform = hnew EditorPlatformOpenGL(window);
			break;
	#endif
	#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: {
			m_Platform = hnew GUIPlatformVulkan(window);
			break;
		}
	#endif
	#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: {
			m_Platform = hnew GUIPlatformMetal(window);
			break;
		}
	#endif
		default:
			HZR_ASSERT(false, "No suitable rendering backend included");
			break;
		}
}
}
