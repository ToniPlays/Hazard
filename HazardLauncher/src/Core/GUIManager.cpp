#include "GUIManager.h"
#include "Hazard.h"
#include "Hazard/Rendering/RenderEngine.h"

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
	io.Fonts->AddFontFromFileTTF("res/fonts/fontawesome-webfont.ttf", 22.0f, &config, icon_ranges);
	io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 16.0f);
	io.Fonts->AddFontFromFileTTF("res/fonts/roboto/Roboto-Black.ttf", 24.0f);

	//Initialize style

	ImUI::StyleManager::LoadStyle(ImUI::Style());

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	InitImGuiPlatform(*m_Window);
	
	m_Manager.LoadFromConfigFile(CONFIG_PATH);
}

void GUIManager::Update()
{

}
void GUIManager::Render()
{
	const ImUI::Style& style = ImUI::StyleManager().GetCurrent();
	m_Platform->BeginFrame();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("HazardLauncher", 0, window_flags);

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, 250);
	DrawSideBar();
	ImGui::NextColumn();
	ImVec2 contentSize = ImGui::GetContentRegionAvail();
	contentSize.y -= 125.0f;

	ImGui::BeginChild("Content", contentSize);

	ImUI::Shift(12.0, 16.0f);
	ImGui::Text(ICON_FK_SEARCH);
	ImGui::SameLine(0, 5.0f);
	ImUI::ShiftY(-6.0f);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 4.0f);
	ImUI::TextField(m_SearchValue, "Search...");
	ImUI::Shift(-4.0, 4.0f);

	const char* headers[] = { "Project", "Version", "Modified" };
	ImUI::Table("ProjectTable", headers, 3, [&]() {
		constexpr float rowHeight = 48.0f;

		for (HazardProject project : m_Manager.GetProjects()) {

			if (!m_SearchValue.empty()) {
				if (!StringUtil::Contains(project.Name, m_SearchValue))
					continue;
			}

			bool clicked = ImUI::TableRowClickable("SomeName", rowHeight);
			ImUI::Group(project.Name.c_str(), [&]() {

				ImUI::ShiftX(4.0f);
				ImUI::Separator({ 2.0, rowHeight }, style.Colors.AxisZ);
				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
				ImGui::SameLine(0, 8.0f);
				ImGui::Text(project.Name.c_str());
				ImGui::PopFont();
				ImGui::TableNextColumn();

				ImUI::ShiftX(8.0f);
				ImGui::Text("0.0.0");

				ImGui::TableNextColumn();
				ImUI::ShiftX(8.0f);
				ImGui::Text("12.2.2022");
				});

			if (clicked) {
				if (m_Manager.OpenProject(project)) {
					WindowCloseEvent e;
					HazardLoop::GetCurrent().OnEvent(e);
				}
				else 
				{
					ImGui::OpenPopup("ErrorPopup");
				}
			}
		}

		});

	ImGui::EndChild();
	ImGui::Columns();
	DrawBottomBar();

	ImGui::End();

	ImGui::PopStyleVar(3);

	if (ImGui::BeginPopup("ErrorPopup")) 
	{
		__debugbreak();
		ImGui::EndPopup();
	}


	ImGui::Render();
	m_Platform->EndFrame();
}
bool GUIManager::OnEvent(Event& e)
{
	return false;
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

void GUIManager::DrawSideBar()
{
	ImVec2 size = ImGui::GetContentRegionAvail();
	size.y -= 125.0f;
	ImGui::BeginChild("Sidebar", size);

	ImGui::EndChild();
}
void GUIManager::DrawBottomBar()
{
	const ImUI::Style& style = ImUI::StyleManager().GetCurrent();
	ImVec2 size = ImGui::GetContentRegionAvail();

	ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);

	ImGui::BeginChild("BottomNavBar", size);
	ImUI::Separator({ size.x, 2.0f }, style.Window.HeaderActive);
	ImUI::Shift(16.0, 32.0f);

	ImUI::ShiftY(4.0f);
	ImGui::Text("Project Location");
	ImGui::SameLine(0, 5.0f);
	ImUI::ShiftY(-4.0f);
	std::string path = m_CurrentProjectPath.string();

	ImGui::SetNextItemWidth(350.0f);
	if (ImUI::TextField(path, "Project path")) {
		m_CurrentProjectPath = path;
	}
	{
		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImUI::ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
		ImGui::SameLine(0, 5.0f);
		if (ImGui::Button("Browse", { 100, 24 })) {
			std::filesystem::path newPath = File::OpenFolderDialog();
			if (!newPath.empty()) 
				m_CurrentProjectPath = newPath;
		}

		ImGui::SetCursorPos({ size.x - 108.0f, size.y - 32.0f });
		if (ImGui::Button("Add project", { 100, 24 }))
		{
			std::filesystem::path newPath = File::OpenFileDialog({ "All files", "* ", "Hazard Project (.hzrproj)", "*.hzrproj" });
			if (!newPath.empty()) {
				if (m_Manager.ImportProject(newPath))
					m_Manager.SaveConfigToFile(CONFIG_PATH);
				else {
					ImGui::OpenPopup("ErrorPopup");
				}
			}
		}
	}
	ImGui::EndChild();
}
