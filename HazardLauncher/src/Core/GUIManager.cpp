#include "GUIManager.h"
#include "Hazard.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "../ImGui_Backend/FontAwesome.h"
#include "Platform/OpenGL/EditorPlatformOpenGL.h"
#include "Platform/Vulkan/EditorPlatformVulkan.h"
#include "Platform/Metal/EditorPlatformMetal.h"

#include "Hazard/ImGUI/UIElements/Table.h"
#include "Hazard/ImGUI/UILibrary.h"


#include "imgui.h"
#include <Directory.h>
#include "Platform/OS.h"

using namespace Hazard;
using namespace HazardRenderer;

void GUIManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	m_Window = &Application::Get().GetModule<RenderContextManager>().GetWindow();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	io.FontDefault = io.Fonts->AddFontFromFileTTF("res/Fonts/roboto/Roboto-Regular.ttf", 16.0f);

	ImFontConfig config;
	config.MergeMode = true;

	static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	io.Fonts->AddFontFromFileTTF("res/Fonts/fontawesome-webfont.ttf", 16.0f, &config, icon_ranges);
	io.Fonts->AddFontFromFileTTF("res/Fonts/roboto/Roboto-Black.ttf", 16.0f);
	io.Fonts->AddFontFromFileTTF("res/Fonts/roboto/Roboto-Black.ttf", 22.0f);

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

	m_EnvVarExists = OS::HasEnv("HAZARD_DIR");

	m_SearchField = ImUI::TextField("");
	m_SearchField.SetHint("Search...");
	m_SearchField.SetIcon((const char*)ICON_FK_SEARCH);

	m_NewProjectPath = ImUI::TextField("");
	m_NewProjectPath.SetHint("Project path");

	m_NewProjectName = ImUI::TextField("");
	m_NewProjectName.SetHint("Project name");
}

void GUIManager::Update()
{

}
void GUIManager::Render()
{
	Renderer::Submit([this]() mutable {
		ImUI::Style& style = ImUI::StyleManager().GetCurrent();
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

		m_SearchField.Render();

		ImUI::Table<HazardProject> table("ProjectTable", ImGui::GetContentRegionAvail());
		table.SetColumns({ "Project", "Version", "Modified" });
		table.RowHeight(36.0f);
		table.RowContent([color = style.Colors.AxisZ](uint32_t index, const HazardProject& project) {
			ImUI::Separator({ 2.0f, 36.0f }, color);
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImGui::SameLine(0, 8.0f);
			ImGui::Text("%s", project.Name.c_str());
			ImGui::PopFont();
			ImGui::TableNextColumn();

			ImUI::ShiftX(8.0f);
			ImGui::Text("0.0.0");

			ImGui::TableNextColumn();
			ImUI::ShiftX(8.0f);
			ImGui::Text("12.2.2022");
		});

		for (const HazardProject& project : m_Manager.GetProjects())
			table.AddRow(project);

		table.Render();

		if (table.DidSelect())
		{
			if (m_Manager.OpenProject(table.SelectedValue()))
			{
				WindowCloseEvent e;
				HazardLoop::GetCurrent().OnEvent(e);
			}
			else
				ImGui::OpenPopup("ErrorPopup");
		}

		ImGui::EndChild();
		ImGui::Columns();
		DrawBottomBar();

		ImGui::End();

		ImGui::PopStyleVar(3);

		if (ImGui::BeginPopup("ErrorPopup"))
		{
			ImGui::EndPopup();
		}

		if (!m_EnvVarExists)
		{
			ImGui::OpenPopup("Install location");
		}
		if (ImGui::BeginPopupModal("Install location"))
		{
			if (ImGui::Button("Set installation folder"))
			{
				std::filesystem::path hazardDir = Directory::OpenFolderDialog();
				if (!hazardDir.empty())
					if (OS::SetEnv("HAZARD_DIR", hazardDir.string().c_str()))
					{
						m_EnvVarExists = true;
						ImGui::CloseCurrentPopup();
					}
			}
			ImGui::EndPopup();
		}

		ImGui::Render();
		m_Platform->EndFrame();
	});
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
			m_Platform = hnew EditorPlatformOpenGL(window);
			break;
		#endif
		#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan:
		{
			m_Platform = hnew EditorPlatformVulkan(window);
			break;
		}
	#endif
	#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal:
		{
			m_Platform = hnew EditorPlatformMetal(window);
			break;
		}
	#endif
		default:
			HZR_ASSERT(false, "No suitable rendering backend included for ImGui");
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
	ImGui::BeginChild("##path", { 350, 28 });

	m_NewProjectPath.Render();

	ImGui::EndChild();
	{
		ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImUI::ScopedColorStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
		ImGui::SameLine(0, 5.0f);
		if (ImGui::Button("Browse", { 100, 28 }))
		{
			std::filesystem::path newPath = Directory::OpenFolderDialog();
			if (!newPath.empty())
				m_NewProjectPath.SetValue(newPath.string());
		}

		ImGui::SameLine(0, 356.0f);
		ImGui::Text("Project name");
		ImGui::SameLine(0, 5.0f);

		ImGui::SetNextItemWidth(250.0f);
		ImGui::PushID("##name");

		m_NewProjectName.Render();
		ImGui::PopID();

		ImGui::SetCursorPos({ size.x - 216.0f, size.y - 32.0f });
		if (ImGui::Button("Create", { 100, 28 }))
		{
			if (!m_NewProjectPath.GetValue().empty())
			{
				HazardProject project = {};
				project.Name = m_NewProjectName.GetValue();
				project.Path = std::filesystem::path(m_NewProjectPath.GetValue()) / project.Name;

				if (m_Manager.CreateProject(project))
					m_Manager.SaveConfigToFile(CONFIG_PATH);

			}
			else HZR_ERROR("Path cannot be empty");
		}
		ImGui::SameLine(0, 8);

		if (ImGui::Button("Add project", { 100, 28 }))
		{
			std::filesystem::path newPath = File::OpenFileDialog({ "All files", "* ", "Hazard Project (.hzrproj)", "*.hzrproj" });
			if (!newPath.empty())
			{
				if (m_Manager.ImportProject(newPath))
					m_Manager.SaveConfigToFile(CONFIG_PATH);
				else
				{
					ImGui::OpenPopup("ErrorPopup");
				}
			}
		}
	}
	ImGui::EndChild();
}
