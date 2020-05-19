#pragma once
#include <HzrEditor.h>
#include "ImGuiLayer.h"
#include "Platform/imgui_impl_glfw.h"
#include "Platform/imgui_impl_opengl3.h"
#include "Gui/Component/EditorStyle.h"

#include "Gui/Component/MainMenu.h"
#include "Gui/Component/UtilityBar.h"

#include "Gui/Widgets/Profiling/Performance.h"
#include "Gui/Widgets/Profiling/Console.h"
#include "Gui/Widgets/Profiling/Profiler.h"
#include "Gui/Widgets/Inspect/AssetManager.h"
#include "Gui/Widgets/Inspect/Hierarchy.h"
#include "Gui/Widgets/Inspect/Inspector.h"
#include "Gui/Widgets/Rendering/Viewport.h"


ColorPicker* ImGuiLayer::colorPicker = nullptr;
std::unordered_map<std::string, GuiLayer*> ImGuiLayer::layers;

ImGuiLayer::ImGuiLayer() : Hazard::Module("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer()
{

}
void ImGuiLayer::OnEnable()
{
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.ConfigDockingWithShift = true;

		// Setup Dear ImGui style
		EditorStyle::InitStyle();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Hazard::Application& app = Hazard::Application::GetCurrent();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	colorPicker = new ColorPicker();
	//Inspect
	PushLayer("Inspector", new Inspector());
	PushLayer("Assets", new AssetManager());
	PushLayer("Hierarchy", new Hierarchy());
	//Rendering
	PushLayer("Viewport", new Viewport());
	//Profiling
	PushLayer("Performance", new Performance());
	PushLayer("Console", new Console());
#ifdef HZR_DEBUG
	PushLayer("Profiler", new Profiler());
#endif // HZR_DEBUG

	
}

void ImGuiLayer::OnDisable()
{

	for (std::pair<std::string, GuiLayer*> layer : layers) {
		delete layer.second;
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnRender()
{
	Begin();
	//Docking stuff
	{
		static bool open = true;
		bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiDockNodeFlags_PassthruCentralNode;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &open, window_flags);

		MainMenu::OnRender();
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		ImGui::End();
	}

	UtilityBar::OnRender();

	for (std::pair<std::string, GuiLayer*> layer : layers) {
		layer.second->OnRender();
	}
	if (colorPicker->isOpen)
		colorPicker->OnRender();
		
	End();
}
void ImGuiLayer::OpenPicker(void(*func)(Hazard::Color color), Hazard::Color color = Hazard::Color())
{
	colorPicker->func = func;
	colorPicker->color = color;
	colorPicker->isOpen = true;
}
void ImGuiLayer::OpenLayer(std::string name)
{
	//layers.at(name)->OpenLayer();
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Hazard::Application& app = Hazard::Application::GetCurrent();
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

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


