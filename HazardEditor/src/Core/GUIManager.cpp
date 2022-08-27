#include "GUIManager.h"
#include "Hazard.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "../ImGui_Backend/FontAwesome.h"
#include "Platform/OpenGL/EditorPlatformOpenGL.h"
#include "Platform/Vulkan/EditorPlatformVulkan.h"
#include "Platform/Metal/EditorPlatformMetal.h"
#include "GUI/AllPanels.h"

#include "imgui.h"

using namespace Hazard;
using namespace HazardRenderer;

void GUIManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	m_Window = &Application::GetModule<RenderEngine>().GetWindow();

	io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 16.0f);
	io.DisplaySize = { (float)m_Window->GetWidth(), (float)m_Window->GetHeight() };

	ImFontConfig config;
	config.MergeMode = true;
	static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	io.Fonts->AddFontFromFileTTF("res/fonts/fontawesome-webfont.ttf", 16.0f, &config, icon_ranges);
	io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 16.0f);
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

	m_PanelManager.AddRenderable<UI::Properties>();
	m_PanelManager.AddRenderable<UI::Hierarchy>();
	m_PanelManager.AddRenderable<UI::GameViewport>();
	m_PanelManager.AddRenderable<UI::Viewport>();
	m_PanelManager.AddRenderable<UI::RenderCommandListPanel>();
	m_PanelManager.AddRenderable<UI::RendererDebugPanel>();
	m_PanelManager.AddRenderable<UI::Console>();
	m_PanelManager.AddRenderable<UI::AssetPanel>();
	m_PanelManager.AddRenderable<UI::ScriptCreatePanel>();
}

void GUIManager::Update()
{
	HZR_PROFILE_FUNCTION();
	m_PanelManager.Update();
}
void GUIManager::Render()
{
	HZR_PROFILE_FUNCTION();
	m_Platform->BeginFrame();

	HZR_PROFILE_FUNCTION("GUIManager::Render() RT");
	m_MainMenuBar.Render();
	{
		using namespace ImUI;
		ScopedStyleVar style(ImGuiStyleVar_FramePadding, ImVec2(16.0f, 8.0f));
		Dockspace::BeginDockspace("MainWorkspace", ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_PassthruCentralNode);
		Dockspace::EndDockspace("MainWorkspace");
	}
	m_PanelManager.Render();

	ImGui::Render();
	m_Platform->EndFrame();
}
bool GUIManager::OnEvent(Event& e)
{
	bool handled = m_MainMenuBar.OnEvent(e);
	handled |= m_PanelManager.OnEvent(e);

	return handled;
}
void GUIManager::InitImGuiPlatform(HazardRenderer::Window& window)
{
	switch (window.GetWindowInfo().SelectedAPI)
	{
#ifdef HZR_INCLUDE_OPENGL
	case RenderAPI::OpenGL:
		m_Platform = new EditorPlatformOpenGL(window);
		break;
#endif
#ifdef HZR_INCLUDE_VULKAN
	case RenderAPI::Vulkan: {
		m_Platform = new EditorPlatformVulkan(window);
		break;
	}
#endif
#ifdef HZR_INCLUDE_METAL
	case RenderAPI::Metal: {
		m_Platform = new EditorPlatformMetal(window);
		break;
	}
#endif
	default:
		HZR_ASSERT(false, "No suitable rendering backend included");
		break;
	}
}
