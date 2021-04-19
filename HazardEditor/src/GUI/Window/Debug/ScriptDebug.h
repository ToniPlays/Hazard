#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard::Scripting;

namespace WindowElement {
	class ScriptDebug : public EditorWindow {
	public:
		ScriptDebug();
		~ScriptDebug();

		void Init() override;
		void OnWindowRender() override;
	};
}