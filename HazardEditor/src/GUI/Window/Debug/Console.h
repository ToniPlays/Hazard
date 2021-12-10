#pragma once

#include "Hazard.h"
#include "GUI/EditorWindow.h"


namespace WindowElement {
	class Console : public EditorWindow {
	public:
		Console();
		~Console() = default;

		void Init() override;
		void OnWindowRender() override;

		void Print(Hazard::Severity severity, const std::string& message);
	private:
		void ClearLog() { messages.clear(); };
		bool LogEnabled(Hazard::Severity severity);

	private:

		bool scrollLockEnable = true;
		bool infoEnable = true;
		bool debugEnable = true;
		bool warningEnable = true;
		bool errorEnable = true;
		bool traceEnable = true;

		std::vector<std::pair<Hazard::Severity, std::string>> messages;
	};
}