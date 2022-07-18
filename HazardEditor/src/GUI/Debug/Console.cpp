
#include "Console.h"

namespace UI
{
	Console::Console() : Panel("Console")
	{

	}
	void Console::Update()
	{

	}
	bool Console::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Console::KeyPressed));
	}
	void Console::OnPanelRender()
	{
		const char* columns[] = { "Type", "Timestamp", "Message" };

		Style& style = StyleManager::GetCurrent();

		ImVec2 consoleSize = ImGui::GetContentRegionAvail();
		consoleSize.y -= 32.0f;

		DrawToolbar({ consoleSize.x, 28.0f });


		UI::Table("Console", columns, 3, consoleSize, [&]() {

			float rowHeight = 24.0f;

			for (auto& message : m_Messages) {
				
				if (!(message.Flags & m_DisplayFlags) && m_DisplayFlags) continue;

				bool clicked = UI::TableRowClickable(message.Message.c_str(), rowHeight);

				Shift(4.0f, 4.0f);
				ImGui::Text(GetMessageType(message));
				ImGui::TableNextColumn();
				Shift(4.0f, 4.0f);
				ImGui::Text("Time");

				ImGui::TableNextColumn();
				Shift(4.0f, 4.0f);
				ImGui::Text(message.Message.c_str());

				Group((const char*)&message, [&]() {
					if (clicked) {
						ImGui::OpenPopup(message.Message.c_str());
						ImVec2 size = ImGui::GetMainViewport()->Size;
						ImGui::SetNextWindowSize({ size.x * 0.5f, size.y * 0.5f });
						ImGui::SetNextWindowPos({ size.x / 2.0f, size.y / 2.5f }, 0, { 0.5, 0.5 });
						m_DetailedPanelOpen = true;
					}
					{
						ScopedStyleVar windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
						ScopedStyleVar framePadding(ImGuiStyleVar_FramePadding, ImVec2(4, 8));
						if (ImGui::BeginPopupModal(message.Message.c_str(), &m_DetailedPanelOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {

							ImGui::Text(message.Description.c_str());
							ImGui::EndPopup();
						}
					}
					});
			}
			});
	}
	bool Console::KeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Escape && m_DetailedPanelOpen) {
			m_DetailedPanelOpen = false;
			return true;
		}
		return false;
	}
	void Console::Clear(bool force)
	{
		for (uint32_t i = 0; i < m_Messages.size(); i++)
		{
			ConsoleMessage& message = m_Messages[i];
			if (message.Flags & MessageFlags_Clearable || force)
			{
				m_Messages.erase(m_Messages.begin() + i, m_Messages.end() + i);
			}
		}
	}
	const char* Console::GetMessageType(const ConsoleMessage& message)
	{
		if (message.Flags & MessageFlags_None)		return "Log";
		if (message.Flags & MessageFlags_Info)		return "Info";
		if (message.Flags & MessageFlags_Warning)	return "Warning";
		if (message.Flags & MessageFlags_Error)		return "Error";
		if (message.Flags & MessageFlags_Fatal)		return "Fatal";
		return "Unknown";
	}
	void Console::DrawToolbar(const ImVec2& size)
	{
		const Style& style = StyleManager::GetCurrent();
		{
			ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::BeginChild("Toolbar", size);
			{
				ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
				if (ImGui::Button("Clear", { 75.0f, 28.0f })) {
					Clear();
				}
			}

			ImGui::SameLine(0, ImGui::GetContentRegionAvailWidth() - 210);
			if (UI::ColoredButton(ICON_FK_INFO_CIRCLE, (m_DisplayFlags & MessageFlags_Info ? style.Frame.FrameColor : style.Window.Header), style.Colors.AxisZ, { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Info;
			}
			ImGui::SameLine();
			if (UI::ColoredButton(ICON_FK_EXCLAMATION_TRIANGLE, (m_DisplayFlags & MessageFlags_Warning ? style.Frame.FrameColor : style.Window.Header), style.Colors.Warning, { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Warning;
			}
			ImGui::SameLine();
			if (UI::ColoredButton(ICON_FK_EXCLAMATION_CIRCLE, (m_DisplayFlags & MessageFlags_Error ? style.Frame.FrameColor : style.Window.Header), style.Colors.AxisX, { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Error;
			}
			ImGui::SameLine();
			if (UI::ColoredButton(ICON_FK_BUG, (m_DisplayFlags & MessageFlags_Fatal ? style.Frame.FrameColor : style.Window.Header), style.Colors.AxisX, { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Fatal;
			}


		}
		ImGui::EndChild();
	}
}