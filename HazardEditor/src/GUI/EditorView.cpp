 #pragma once
#include <hzreditor.h>
#include "EditorView.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "GLFW/imgui_impl_glfw.h"
#include "GLFW/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "Library/Style.h"
#include "Library/Layout/Dockspace.h"
#include "Window/AllWindows.h"

#include "GUI/Library/FontAwesome.h"

using namespace Hazard;

namespace WindowElement {

	EditorView* EditorView::s_Instance = nullptr;

	EditorView::EditorView() : Module::Module("EditorViews")
	{
		m_Elements = std::vector<RenderableElement*>();
		s_Instance = this;
	}
	EditorView::~EditorView()
	{

	}
	void EditorView::Init()
	{
		bool found = false;
		m_Context = &Hazard::Application::GetModule<Rendering::RenderContext>(found);
		if (!found) {
			SetActive(false);
			HZR_WARN("EditorView unable to start without RenderContext");
			return;
		}
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
		io.ConfigDockingWithShift = true;

		GLFWwindow* window = static_cast<GLFWwindow*>(m_Context->GetWindow().GetNativeWindow());

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
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		Appereance::Style::Init();

		PushRenderable<WelcomePopup>();
		PushRenderable<MenuBar>();

		PushRenderable<GameViewport>();
		PushRenderable<Viewport>();
		PushRenderable<FileView>();

		PushRenderable<Properties>();
		PushRenderable<Hierarchy>();
		PushRenderable<Performance>();

		PushRenderable<Console>();
		PushRenderable<EngineAssets>();
		PushRenderable<ScriptDebug>();

		PushRenderable<ShaderEditorWindow>();
	}  
	void EditorView::Render()
	{
		BeginFrame();
		{
			for (RenderableElement* element : m_Elements) {
				element->OnMenuRender();
			}
			WindowLayout::Dockspace::Begin("MainDockSpace");
			WindowLayout::Dockspace::End();
			m_Toolbar.OnRender();
		}
		for (RenderableElement* element : m_Elements) {
			element->OnRender();
		}
		EndFrame();
	}
	bool EditorView::OnEvent(Event& e)
	{
		for (RenderableElement* element : m_Elements) {
			if (element->OnEvent(e)) 
				return true;
		}
		return false;
	}
	void EditorView::Close()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void EditorView::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
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

		// Rendering 
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}