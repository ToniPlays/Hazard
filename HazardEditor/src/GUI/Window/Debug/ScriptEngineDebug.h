#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard::Scripting;

namespace WindowElement {
	class ScriptEngineDebug : public EditorWindow {
	public:
		ScriptEngineDebug();
		~ScriptEngineDebug() = default;

		void Init() override;
		void OnWindowRender() override;

	private:

	};
}