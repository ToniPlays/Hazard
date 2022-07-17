#pragma once

#include "GUI/Core/Panel.h"

enum MessageFlags : uint32_t {
	MessageFlags_None = BIT(0),
	MessageFlags_Info = BIT(1),
	MessageFlags_Warning = BIT(2),
	MessageFlags_Error = BIT(3),
	MessageFlags_Fatal = BIT(4),
	MessageFlags_Clearable = BIT(5)
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
	private:
		std::vector<ConsoleMessage> m_Messages;
		MessageFlags m_DisplayFlags;
		bool m_DetailedPanelOpen = false;
	};
}