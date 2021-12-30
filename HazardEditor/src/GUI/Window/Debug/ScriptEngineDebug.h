#pragma once
#include <hzrpch.h>
#include "GUI/EditorWindow.h"
#include "Hazard/Scripting/ScriptRegistry.h"
#include "Hazard/Scripting/Attributes/AllAttributes.h"

namespace WindowElement {
	class ScriptEngineDebug : public EditorWindow {
	public:
		ScriptEngineDebug();
		~ScriptEngineDebug() = default;

		void Init() override;
		void OnWindowRender() override;

		void ProcessFields(std::vector<Hazard::Scripting::ScriptFieldMetadata> fields);
		void ProcessMethods(std::vector<Hazard::Scripting::ScriptMethodMetadata> method);
		template<typename T>
		void TryProcessAttribute(Hazard::Scripting::ScriptMetadata meta);
		template<typename T>
		void TryProcessAttribute(Hazard::Scripting::ScriptFieldMetadata field);
		template<typename T>
		void TryProcessAttribute(Hazard::Scripting::ScriptMethodMetadata method);
		template<typename T>
		void ProcessAttribute(T attribute);
	};
}