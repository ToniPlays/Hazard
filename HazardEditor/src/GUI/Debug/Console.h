#pragma once

#include "GUI/Core/Panel.h"

enum MessageFlags : uint32_t {
	MessageFlags_None = BIT(0),
	MessageFlags_Debug = BIT(1),
	MessageFlags_Info = BIT(2),
	MessageFlags_Warning = BIT(3),
	MessageFlags_Error = BIT(4),
	MessageFlags_Fatal = BIT(5),
	MessageFlags_Clearable = BIT(6)
};

namespace UI {

	struct ConsoleMessage {
		std::string Message;
		std::string Description;
		uint32_t Flags;
	};

	class Console : public Panel {
	public:
		Console();
		~Console() = default;

		void Update() override;
		bool OnEvent(Event& e) override;
		void OnPanelRender();

		bool KeyPressed(KeyPressedEvent& e);

		void AddMessage(const ConsoleMessage& message) {
			m_Messages.push_back(message);
		}
		void Clear(bool force = false);
		const char* GetMessageType(const ConsoleMessage& message);

	private:
		void DrawToolbar(const ImVec2& size);
		ImVec4 GetMessageColor(const uint32_t& flag);

	private:
		std::vector<ConsoleMessage> m_Messages;
		uint32_t m_DisplayFlags = 0;
		bool m_DetailedPanelOpen = false;
	};
}