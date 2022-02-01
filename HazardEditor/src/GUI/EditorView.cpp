
#include <hzrpch.h>
#include "EditorView.h"

#include "Library/Style.h"
#include "Library/Layout/Dockspace.h"
#include "Window/AllWindows.h"

#include "Platform/GLFW/FontAwesome.h"
#include "Platform/RenderPlatforms.h"

#include <GLFW/glfw3.h>

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
		for (auto element : m_Elements) {
			delete element;
		}
		m_Elements.clear();
	}
	void EditorView::Init()
	{
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
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags|= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			 // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 16.0f);
		io.DisplaySize = { 1280, 720 };

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

		InitImGuiPlatform(m_Context->GetWindow());

		Style::InitTheme(Theme::Dark);

		PushRenderable<MenuBar>();
		PushRenderable<EditorMainTab>();
		PushRenderable<NodeGraphTab>();

		PushRenderable<WorldEnvironmentData>();

		PushRenderable<Performance>();
		PushRenderable<EngineAssets>();

		PushRenderable<Profiler>();
		//PushRenderable<WelcomePopup>();

		PushRenderable<SettingsView>();
		PushRenderable<BuildWindow>();
		PushRenderable<ScriptEngineDebug>();
		PushRenderable<AudioEngineDebug>();
	}
	void EditorView::Update()
	{
		HZR_PROFILE_FUNCTION();
		for (RenderableElement* element : m_Elements) {
			element->OnUpdate();
		}
	}
	void EditorView::Render()
	{
		HZR_PROFILE_FUNCTION();

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
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT(EditorView::OnWindowFocus));

		for (RenderableElement* element : m_Elements) {
			if (element->OnEvent(e)) {
				return true;
			}
		}
		return m_Toolbar.OnEvent(e);
	}
	void EditorView::Close()
	{
		for (uint32_t i = 0; i < m_Elements.size(); i++) {
			delete m_Elements[i];
		}
		m_Elements.clear();

		delete m_Renderer;
		ImGui::DestroyContext();
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

		// Rendering 
		ImGui::Render();
		m_Renderer->EndFrame();
	}
	void EditorView::InitImGuiPlatform(Rendering::Window& window)
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		switch (m_Context->GetCurrentAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:
			m_Renderer = new EditorPlatformOpenGL(nativeWindow);
			break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: {
			Rendering::Vulkan::VulkanContext* context = static_cast<Vulkan::VulkanContext*>(window.GetContext());
			m_Renderer = new EditorPlatformVulkan(nativeWindow, context);
			break;
		}
#endif
		default:
			m_Renderer = nullptr;
			break;
		}
	}
	bool EditorView::OnWindowFocus(WindowFocusEvent& e)
	{
		if(e.GetFocus())
			Hazard::Scripting::ScriptCommand::ReloadAssemblies();
		return true;
	}
}
