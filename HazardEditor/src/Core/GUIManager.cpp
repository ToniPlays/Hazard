#include "GUIManager.h"
#include "Hazard.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "GUI/Core/Dockspace.h"
#include "GUI/Core/StyleManager.h"

#include "../ImGui_Backend/FontAwesome.h"
#include "Platform/OpenGL/EditorPlatformOpenGL.h"
#include "Platform/Vulkan/EditorPlatformVulkan.h"
#include "Platform/Metal/EditorPlatformMetal.h"

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

	UI::StyleManager::LoadStyle(UI::Style());

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	InitImGuiPlatform(*m_Window);

	//Initialize style
}

void GUIManager::Update()
{
	m_Platform->BeginFrame();
}
void GUIManager::Render()
{
	m_MainMenuBar.Render();
	{
		using namespace UI;
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
	m_MainMenuBar.OnEvent(e);
	m_PanelManager.OnEvent(e);
	return true;
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
