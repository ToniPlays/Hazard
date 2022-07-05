#include "GUIManager.h"
#include "Hazard.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Platform/GLFW/FontAwesome.h"
#include "Platform/OpenGL/EditorPlatformOpenGL.h"
#include "Platform/Vulkan/EditorPlatformVulkan.h"

#include "imgui.h"

using namespace Hazard;
using namespace HazardRenderer;

void GUIManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
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
	m_PanelManager.Render();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

	ImGui::Render();
	m_Platform->EndFrame();
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
	default:
		HZR_ASSERT(false, "No suitable rendering backend included");
		break;
	}
}
