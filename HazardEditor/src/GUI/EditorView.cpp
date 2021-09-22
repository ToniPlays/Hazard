 #pragma once
#include <hzreditor.h>
#include "EditorView.h"

#include "Library/Style.h"
#include "Library/Layout/Dockspace.h"
#include "Window/AllWindows.h"

#include "GUI/Library/FontAwesome.h"
#include "Platform/OpenGL/EditorPlatformOpenGL.h"
#include "Platform/Vulkan/EditorPlatformVulkan.h"

using namespace Hazard;

namespace WindowElement {

	EditorView* EditorView::s_Instance = nullptr;

	EditorView::EditorView() : Module::Module("EditorViews")
	{
		m_Elements = std::vector<RenderableElement*>();
		s_Instance = this;
	}
	void EditorView::Init()
	{
		HZR_PROFILE_FUNCTION();

		if (!Hazard::Application::HasModule<Rendering::RenderContext>()) {
			SetActive(false);
			HZR_WARN("EditorView unable to start without RenderContext");
			return;
		}

		m_Context = &Hazard::Application::GetModule<Rendering::RenderContext>();
		SetActive(true);


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags  |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags|= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
		io.ConfigFlags  |= ImGuiConfigFlags_DockingEnable;			 // Enable Docking
		io.ConfigFlags  |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;

		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 16.0f);

		ImFontConfig config;
		config.MergeMode = true;
		static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		io.Fonts->AddFontFromFileTTF("res/fonts/fontawesome-webfont.ttf", 16.0f, &config, icon_ranges);
		io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 16.0f);

		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		InitImGuiPlatform(m_Context->GetWindow());
		
		Style::InitTheme(Theme::Dark);

		PushRenderable<MenuBar>();
		PushRenderable<EditorMainTab>();
		PushRenderable<NodeGraphTab>();
		
		PushRenderable<WorldEnvironmentData>();

		PushRenderable<Performance>();
		PushRenderable<EngineAssets>();

		PushRenderable<Profiler>();
		PushRenderable<WelcomePopup>();

		PushRenderable<ExportView>();
		PushRenderable<SettingsView>();
	}  
	void EditorView::Render()
	{
		BeginFrame();
		{
			for (RenderableElement* element : m_Elements) {
				element->OnMenuRender();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Style::GetStyle().dockPadding);
			WindowLayout::Dockspace::Begin("MainDockSpace", ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoSplit);
			WindowLayout::Dockspace::End("MainDockSpace");
			ImGui::PopStyleVar();

			for (RenderableElement* element : m_Elements) 
			{
				element->OnRender();
			}
		}

		EndFrame();
	}
	bool EditorView::OnEvent(Event& e)
	{
		if (EditorView::s_Instance == nullptr) 
			return false;

		for (RenderableElement* element : m_Elements) {
			if (element->OnEvent(e)) {
				return true;
			}
		}
		return m_Toolbar.OnEvent(e);
	}
	void EditorView::Close()
	{
		HZR_PROFILE_FUNCTION();
		m_Renderer->Close();
	}
	void EditorView::BeginFrame()
	{
		m_Renderer->BeginFrame();
		ImGui::NewFrame();

		for (RenderableElement* element : m_Elements) {
			element->OnFrameBegin();
		}
	}
	void EditorView::EndFrame()
	{
		for (RenderableElement* element : m_Elements) {
			element->OnFrameEnd();
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_Context->GetWindow().GetWidth(), (float)m_Context->GetWindow().GetHeight());

		ImGui::Render();
		m_Renderer->EndFrame();
	}
	void EditorView::InitImGuiPlatform(Rendering::Window& window)
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		switch (m_Context->GetCurrentAPI())
		{
		case RenderAPI::OpenGL:
			m_Renderer = new EditorPlatformOpenGL(nativeWindow);
			break;
		case RenderAPI::Vulkan:
			Rendering::Vulkan::VKContext* context = static_cast<Vulkan::VKContext*>(window.GetContext());
			m_Renderer = new EditorPlatformVulkan(nativeWindow, context);
			break;
		}
	}
}