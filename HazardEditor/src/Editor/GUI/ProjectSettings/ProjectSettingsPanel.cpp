
#include "ProjectSettingsPanel.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"
#include "Hazard/ImGUI/UIElements/Table.h"
#include "Hazard/ImGUI/ScopedVar.h"
#include <Hazard/Input/InputSchema.h>
#include <Hazard/Input/InputManager.h>

#include <Project/ProjectManager.h>
#include <Core/HazardEditor.h>

#include "Pages/ApplicationGeneral.h"
#include "Pages/BuildSettings.h"

namespace UI
{
	using namespace Hazard;

	ProjectSettingsPanel::ProjectSettingsPanel() : Hazard::ImUI::Panel("Project Settings", false)
	{
		m_SearchField = ImUI::TextField("");
		m_SearchField.SetIcon((const char*)ICON_FK_SEARCH);
		m_SearchField.SetHint("Search...");

		m_Pages[GENERAL_APP_PROPERTIES] = new ApplicationGeneral();
		m_Pages[RUNTIME_BUILD_SETTINGS] = new BuildSettings();

		OpenPage(GENERAL_APP_PROPERTIES);
	}

	ProjectSettingsPanel::~ProjectSettingsPanel()
	{
		for (auto& [index, page] : m_Pages)
			delete page;
	}

	void ProjectSettingsPanel::OnPanelRender()
	{
		RenderNavbar();
		ImGui::SameLine();

		ImVec2 size = ImGui::GetContentRegionAvail();

		ImGui::BeginChild("##projectsettings", { size.x, size.y }, ImGuiWindowFlags_AlwaysUseWindowPadding);
		m_SearchField.Render();

		auto page = m_Pages[m_CurrentPage];
		if (page)
		{
			RenderPageHeader(page->GetPageTitle(), page->GetPageDescription());
			page->RenderPage();
		}

		ImGui::EndChild();
	}
	bool ProjectSettingsPanel::OnEvent(Event& e)
	{
		if (m_PendingType == InputDeviceType::None) return false;

		EventDispatcher dispatcher(e);
		bool handled = dispatcher.DispatchUntilHandled<KeyPressedEvent>(BIND_EVENT(ProjectSettingsPanel::KeyPressed));
		handled |= dispatcher.DispatchUntilHandled<MouseButtonPressedEvent>(BIND_EVENT(ProjectSettingsPanel::MouseButtonPressed));
		handled |= dispatcher.DispatchUntilHandled<GamepadButtonPressedEvent>(BIND_EVENT(ProjectSettingsPanel::GamepadButtonPressed));
		handled |= dispatcher.DispatchUntilHandled<GamepadAxisMovedEvent>(BIND_EVENT(ProjectSettingsPanel::GamepadAxisMoved));

		return handled;
	}
	bool ProjectSettingsPanel::KeyPressed(KeyPressedEvent& e)
	{
		if (m_PendingType != InputDeviceType::Keyboard) return false;

		m_PreviousKey = e.GetKeyCode();
		m_PendingAxis->KeyCode = e.GetKeyCode();

		m_PendingType = InputDeviceType::None;

		m_PendingAxis = nullptr;
		return true;
	}
	bool ProjectSettingsPanel::MouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (m_PendingType != InputDeviceType::Mouse) return false;

		m_PreviousMouseKey = e.GetMouseButton();
		m_PendingAxis->KeyCode = e.GetMouseButton();

		m_PendingType = InputDeviceType::None;

		m_PendingAxis = nullptr;
		return true;
	}
	bool ProjectSettingsPanel::GamepadButtonPressed(GamepadButtonPressedEvent& e)
	{
		if (m_PendingType != InputDeviceType::Gamepad) return false;

		m_PreviousGamepadKey = e.GetGamepadButton();
		m_PendingAxis->KeyCode = e.GetGamepadButton();

		m_PendingAxis = nullptr;
		m_PendingType = InputDeviceType::None;

		return true;
	}

	void ProjectSettingsPanel::WaitForInput(InputDeviceType device, InputAxis* axis)
	{
		if (!axis) return;

		m_PreviousKey = Key::None;
		m_PreviousMouseKey = Mouse::None;
		m_PreviousGamepadKey = Gamepad::None;
		m_PendingAxis = axis;
		m_PendingType = device;
	}

	bool ProjectSettingsPanel::GamepadAxisMoved(GamepadAxisMovedEvent& e)
	{
		if (m_PendingType != InputDeviceType::Gamepad || glm::abs(e.GetAxisValue()) <= 0.25f) return false;

		m_PreviousGamepadKey = e.GetGamepadAxis();
		m_PendingAxis->KeyCode = e.GetGamepadAxis();

		m_PendingAxis = nullptr;
		return false;
	}

	void ProjectSettingsPanel::RenderNavbar()
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImVec2 size = ImGui::GetContentRegionAvail();

		ImUI::ScopedStyleColor color(ImGuiCol_ChildBg, style.BackgroundColor);

		ImUI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2({ 8, 8 }));
		ImGui::BeginChild("##navbar", { 225, size.y }, ImGuiWindowFlags_AlwaysUseWindowPadding);
		{
			float width = ImGui::GetContentRegionAvail().x;

			ImUI::ScopedStyleVar alignment(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("General");
			ImGui::PopFont();

			if (ImGui::Button("Application properties", { width, 32 }))
				OpenPage(GENERAL_APP_PROPERTIES);
			if (ImGui::Button("Input settings", { width, 32 }))
				OpenPage(GENERAL_INPUT_SETTINGS);

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Runtime");
			ImGui::PopFont();

			if (ImGui::Button("Build settings", { width, 32 }))
				OpenPage(RUNTIME_BUILD_SETTINGS);

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Scripting");
			ImGui::PopFont();

			if (ImGui::Button("Preprocessor defines", { width, 32 }))
				OpenPage(SCRIPTING_PREPROCESSORS);
			if (ImGui::Button("Dependencies", { width, 32 }))
				OpenPage(SCRIPTING_DEPENDENCIES);
			if (ImGui::Button("Execution order", { width, 32 }))
				OpenPage(SCRIPTING_EXECUTION_ORDER);


			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Rendering");
			ImGui::PopFont();

			if (ImGui::Button("Quality settings", { width, 32 }))
				OpenPage(RENDERING_QUALITY_SETTINGS);
			if (ImGui::Button("Lighting settings", { width, 32 }))
				OpenPage(RENDERING_LIGHTING_SETTINGS);
			if (ImGui::Button("Shadow settings", { width, 32 }))
				OpenPage(RENDERING_SHADOW_SETTINGS);

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImUI::MenuHeader("Physics");
			ImGui::PopFont();

			if (ImGui::Button("Physics properties", { width, 32 }))
				OpenPage(PHYSICS_PROPERTIES);

			if (ImGui::Button("Collision layers", { width, 32 }))
				OpenPage(PHYSICS_COLLISION_LAYERS);
		}
		ImGui::EndChild();
	}

	void ProjectSettingsPanel::RenderPageHeader(const char* title, const char* description)
	{
		ImUI::Shift(8.0, 8.0f);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		ImGui::Text("%s", title);
		ImGui::PopFont();

		ImUI::Shift(8.0, 8.0f);
		ImGui::Text("%s", description);

		ImUI::Shift(8.0, 8.0f);
	}

	void ProjectSettingsPanel::DrawInputSettingsPanel()
	{
		//DrawInputSourceTable();

		//auto& schema = InputManager::GetSchema();

		ImUI::Treenode bindings("Bindings", true);
		bindings.Content([&]() mutable {
			//DrawInputBindingsTable(schema.Bindings, this);
		});
		bindings.Render();

		ImUI::Treenode axis("Axis", true);
		axis.Content([&]() mutable {
			//DrawInputAxisTable(schema.Bindings, this);
		});
		axis.Render();

		ImUI::Treenode mouseProperties("Mouse properties", true);
		mouseProperties.Content([this]() {

			if (m_PreviousKey != Key::None)
				ImGui::Text("Pressed key: %s", Key::ToString(m_PreviousKey));

			if (m_PreviousMouseKey != Mouse::None)
				ImGui::Text("Pressed mouse button: %s", Mouse::ToString(m_PreviousMouseKey));

			if (m_PreviousGamepadKey != Gamepad::None && m_PreviousGamepadKey < Gamepad::AxisFirst)
				ImGui::Text("Pressed gamepad button: %s", Gamepad::ToString(m_PreviousGamepadKey));

			else if (m_PreviousGamepadKey != Gamepad::None)
				ImGui::Text("Moved gamepad axis: %s (%.2f)", Gamepad::ToString(m_PreviousGamepadKey), 0.0f);
		});
		mouseProperties.Render();
	}

	void ProjectSettingsPanel::DrawScriptingPreprocessors()
	{
		/*
		HazardEditorApplication& app = (HazardEditorApplication&)Application::Get();
		auto& predefined = app.GetScriptManager().GetPreprocessorDefines();
		auto& manager = Application::Get().GetModule<ProjectManager>();
		auto& project = manager.GetProject();

		DrawScriptDefineList(&project.GetScriptingSettings().Defines);

		ImUI::Shift(4.0f, 4.0f);
		ImGui::Text("Other preprocessors");
		ImUI::ShiftY(4.0f);
		DrawScriptPredefinedList(predefined);
		*/
	}
	void ProjectSettingsPanel::OpenPage(uint32_t page)
	{
		if (m_Pages[m_CurrentPage])
			m_Pages[m_CurrentPage]->OnClose();
		m_CurrentPage = (SettingsPageFlags)page;
		if (m_Pages[m_CurrentPage])
			m_Pages[m_CurrentPage]->OnOpen();
	}
}
