
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

		UI::Table("Console", columns, 3, [&]() {

			float rowHeight = 24.0f;

			for (auto& message : m_Messages) {

				bool clicked = UI::TableRowClickable(message.Message.c_str(), rowHeight);

				Shift(4.0f, 4.0f);
				ImGui::Text("Info");
				ImGui::TableNextColumn();
				Shift(4.0f, 4.0f);
				ImGui::Text("Time");

				ImGui::TableNextColumn();
				Shift(4.0f, 4.0f);
				ImGui::Text(message.Message.c_str());

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
}