#pragma once

#include <hzrpch.h>
#include "GUI/EditorWindow.h"
#include "Severity.h"

namespace WindowElement {
	class Console : public EditorWindow {
	public:
		Console();
		~Console() = default;

		void Init() override;
		void OnWindowRender() override;

		void Print(Severity severity, const std::string& message);
	private:
		void ClearLog() { messages.clear(); };
		bool LogEnabled(Severity severity);

	private:

		bool scrollLockEnable = true;
		bool infoEnable = true;
		bool debugEnable = true;
		bool warningEnable = true;
		bool errorEnable = true;
		bool traceEnable = true;

		std::vector<std::pair<Severity, std::string>> messages;
	};
}