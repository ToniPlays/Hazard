#pragma once
#include <hzreditor.h>
#include "Editor.h"
#include "All.h"
#include "Editor/Core/ComponentRegister.h"

Editor* Editor::instance = nullptr;

Editor::Editor() : Module("EditorGUI")
{
	mainMenu.OnCreate();

	ModuleHandler::PushModule(new ComponentRegister());

	PushLayer(new Performance());
	PushLayer(new Inspector());
	PushLayer(new Profiler());
	PushLayer(new Console());
	PushLayer(new EngineAssets());
	PushLayer(new Viewport());
	PushLayer(new Hierarchy());
	PushLayer(new NewProject());
}
Editor::~Editor()
{

}
bool Editor::OnEnabled() {
	{
		
		if (Editor::instance != nullptr) 
			delete Editor::instance;

		Editor::instance = this;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigDockingWithShift = true;
		// Setup Dear ImGui style
		Style::InitDarkTheme();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		RenderEngine* renderer = ModuleHandler::GetModule<Hazard::RenderEngine>();

		if (renderer == nullptr) {
			SetActive(false);
			return false;
		}
		
		GLFWwindow* window = static_cast<GLFWwindow*>(renderer->GetWindow().GetNativeWindow());

		io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Regular.ttf", 16.0f);
		io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 16.0f);

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		
		return true;
	}
}

void Editor::PushLayer(Layer* layer)
{
	if (layer->OnEnabled()) {
		Register(layer);
	}
	else Debug::Warn("Unable to set layer: " + layer->GetName());
}
void Editor::Register(Layer* layer) {
	if(layer->MenuPlacement() != "")
		mainMenu.RegisterMenuItem(layer->MenuPlacement(), new Callback(layer->GetName(), [](void* item) {
			Layer* layer = static_cast<Layer*>(item);
			layer->SetLayerOpen(true);
		}, layer));

	layers.push_back(layer);
}

void Editor::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Editor::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Hazard::RenderEngine* renderer = Hazard::ModuleHandler::GetModule<Hazard::RenderEngine>();
	io.DisplaySize = ImVec2((float)renderer->GetWindow().GetWidth(), (float)renderer->GetWindow().GetHeight());

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

void Editor::Render() {
	PROFILE_FN()
	Begin();
	//Docking stuff
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();


		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		Style::GetStyle()->WindowMinSize = ImVec2(315.0f, 100.0f);
		mainMenu.OnRender();
		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		
		Style::GetStyle()->WindowMinSize.x = 32.0f;
		ImGui::End();
	}

	//UtilityBar::OnRender();

	for (Layer* layer : layers) layer->Render();

	End();
	PROFILE_FN_END();
}

bool Editor::OnDisabled()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}
