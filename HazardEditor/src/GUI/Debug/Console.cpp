
#include "Console.h"
#include "Core/MessageFlags.h"


namespace UI
{
	Console::Console() : Panel("Console")
	{

	}
	bool Console::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Console::KeyPressed));
	}
	void Console::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		const char* columns[] = { "Type", "Timestamp", "Message" };

		ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		ImVec2 consoleSize = ImGui::GetContentRegionAvail();
		consoleSize.y -= 32.0f;

		DrawToolbar({ consoleSize.x, 28.0f });

		ImUI::Table("Console", columns, 3, consoleSize, [&]() {

			float rowHeight = 24.0f;
			for (auto& message : m_Messages) 
			{
				if (!(message.Flags & m_DisplayFlags) && m_DisplayFlags) continue;

				bool clicked = ImUI::TableRowClickable(message.Message.c_str(), rowHeight);

				ImUI::Group((const char*)&message, [&]() {
					ImUI::Separator({ 4.0, rowHeight }, GetMessageColor(message.Flags));
					ImGui::SameLine();
					ImGui::Text(GetMessageType(message));
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text("Time");

					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text(message.Message.c_str());

					if (clicked) {
						ImGui::OpenPopup(message.Message.c_str());
						ImVec2 size = ImGui::GetMainViewport()->Size;
						ImGui::SetNextWindowSize({ size.x * 0.5f, size.y * 0.5f });
						ImGui::SetNextWindowPos({ size.x / 2.0f, size.y / 2.5f }, 0, { 0.5, 0.5 });
						m_DetailedPanelOpen = true;
					}
					{
						ImUI::ScopedStyleVar windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
						ImUI::ScopedStyleVar framePadding(ImGuiStyleVar_FramePadding, ImVec2(4, 8));
						ImUI::ScopedStyleColor color(ImGuiCol_PopupBg, style.ChildBackgroundColor);
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
		std::vector<ConsoleMessage> messages;
		uint32_t size = m_Messages.size();
		for (uint32_t i = 0; i < size; i++)
		{
			ConsoleMessage& message = m_Messages[i];
			if (!(message.Flags & MessageFlags_Clearable || force))
			{
				messages.push_back(message);
			}
		}
		m_Messages = messages;
	}
	const char* Console::GetMessageType(const ConsoleMessage& message)
	{
		if (message.Flags & MessageFlags_None)		return "Log";
		if (message.Flags & MessageFlags_Debug)		return "Debug";
		if (message.Flags & MessageFlags_Info)		return "Info";
		if (message.Flags & MessageFlags_Warning)	return "Warning";
		if (message.Flags & MessageFlags_Error)		return "Error";
		if (message.Flags & MessageFlags_Fatal)		return "Fatal";
		return "Unknown";
	}
	void Console::DrawToolbar(const ImVec2& size)
	{
		using namespace ImUI;
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
			ImGui::SameLine(0, 5);
			if (ColoredButton("Clear on build", (m_ClearOnBuild ? style.Frame.FrameColor : style.Window.Header), style.Window.Text, { 100.0f, 28.0f })) {
				m_ClearOnBuild = !m_ClearOnBuild;
			}

			ImGui::SameLine(0, 5);
			if (ColoredButton("Clear on play", (m_ClearOnPlay ? style.Frame.FrameColor : style.Window.Header), style.Window.Text, { 100.0f, 28.0f })) {
				m_ClearOnPlay = !m_ClearOnPlay;
			}

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 136, 0);
			if (ColoredButton((const char*)ICON_FK_PAPERCLIP, (m_DisplayFlags & MessageFlags_Debug ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Debug), { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Debug;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_INFO_CIRCLE, (m_DisplayFlags & MessageFlags_Info ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Info), { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Info;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_EXCLAMATION_TRIANGLE, (m_DisplayFlags & MessageFlags_Warning ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Warning), { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Warning;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_EXCLAMATION_CIRCLE, (m_DisplayFlags & MessageFlags_Error ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Error), { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Error;
			}
			ImGui::SameLine();
			if (ColoredButton((const char*)ICON_FK_BUG, (m_DisplayFlags & MessageFlags_Fatal ? style.Frame.FrameColor : style.Window.Header), GetMessageColor(MessageFlags_Fatal), { 28, 28 })) {
				m_DisplayFlags ^= MessageFlags_Fatal;
			}


		}
		ImGui::EndChild();
	}
	ImVec4 Console::GetMessageColor(const uint32_t& flag)
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		if (flag & MessageFlags_None)		return style.Colors.AxisZ;
		if (flag & MessageFlags_Info)		return style.Colors.AxisZ;
		if (flag & MessageFlags_Debug)		return style.Colors.AxisY;
		if (flag & MessageFlags_Warning)	return style.Colors.Warning;
		if (flag & MessageFlags_Error)		return style.Colors.AxisX;
		if (flag & MessageFlags_Fatal)		return style.Colors.AxisX;

		return style.Window.Header;
	}
}