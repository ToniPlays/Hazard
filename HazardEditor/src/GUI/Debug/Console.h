#pragma once

#include "Hazard.h"
#include "Core/EditorEvent.h"


namespace UI {

	struct ConsoleMessage {
		std::string Message;
		std::string Description;
		uint32_t Flags;
	};

	class Console : public Hazard::ImUI::Panel {
	public:
		Console();
		~Console() = default;

		bool OnEvent(Event& e) override;
		void OnPanelRender() override;

		bool KeyPressed(KeyPressedEvent& e);

		void AddMessage(const ConsoleMessage& message) 
		{
			m_Messages.push_back(message);
		}
		void Clear(bool force = false);
		const char* GetMessageType(const ConsoleMessage& message);

		bool ClearOnPlay() { return m_ClearOnPlay; }
		bool ClearOnBuild() { return m_ClearOnBuild; }

	private:
		void DrawToolbar(const ImVec2& size);
		ImVec4 GetMessageColor(const uint32_t& flag);

	private:
		std::vector<ConsoleMessage> m_Messages;
		uint32_t m_DisplayFlags = 0;
		bool m_DetailedPanelOpen = false;
		bool m_ClearOnBuild = true;
		bool m_ClearOnPlay = true;
	};
}
