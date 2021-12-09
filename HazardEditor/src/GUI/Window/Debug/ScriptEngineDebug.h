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

		void ProcessFields(std::vector<ScriptFieldMetadata> fields);
		void ProcessMethods(std::vector<ScriptMethodMetadata> method);
		template<typename T>
		void TryProcessAttribute(ScriptMetadata meta);
		template<typename T>
		void TryProcessAttribute(ScriptFieldMetadata field);
		template<typename T>
		void TryProcessAttribute(ScriptMethodMetadata method);
		template<typename T>
		void ProcessAttribute(T attribute);
	};
}