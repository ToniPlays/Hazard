
#include "ScriptEngineDebug.h"

#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"
#include "GUI/EditorView.h"
#include "GUI/Window/Tabs/EditorMainTab.h"
#include "Hazard/Scripting/ScriptRegistry.h"
#include "Hazard/Scripting/Attributes/AllAttributes.h"

namespace WindowElement
{
	ScriptEngineDebug::ScriptEngineDebug() : EditorWindow("Script engine debugger")
	{

	}
	void ScriptEngineDebug::Init()
	{

	}
	void ScriptEngineDebug::ProcessFields(std::vector<Hazard::Scripting::ScriptFieldMetadata> fields)
	{
		ImGuiTreeNodeFlags scriptFlags = ImGuiTreeNodeFlags_Framed;
		WindowLayout::Layout::Treenode("Class fields", scriptFlags, [&]() {
			for (auto& field : fields)
			{
				std::stringstream ss;
				ss << FieldVisibilityToChar(field.Visibility) << " ";
				ss << FieldTypeToChar(field.Type) << " " << field.Name;

				WindowLayout::Layout::Treenode(ss.str().c_str(), scriptFlags, [&]() {
					ImGui::Text("Attributes:");

					TryProcessAttribute<Hazard::Scripting::ShowInPropertiesAttribute>(field);
					TryProcessAttribute<Hazard::Scripting::HideInPropertiesAttribute>(field);
					TryProcessAttribute<Hazard::Scripting::SliderAttribute>(field);
					TryProcessAttribute<Hazard::Scripting::RangeAttribute>(field);
					TryProcessAttribute<Hazard::Scripting::HeaderAttribute>(field);
					TryProcessAttribute<Hazard::Scripting::TooltipAttribute>(field);
					TryProcessAttribute<Hazard::Scripting::TextAreaAttribute>(field);

				});
			}
			});
	}
	void ScriptEngineDebug::ProcessMethods(std::vector<Hazard::Scripting::ScriptMethodMetadata> methods)
	{
		ImGuiTreeNodeFlags scriptFlags = ImGuiTreeNodeFlags_Framed;
		WindowLayout::Layout::Treenode("Class methods", scriptFlags, [&]() {
			for (auto& method : methods)
			{
				std::stringstream ss;
				ss << FieldVisibilityToChar(method.Visibility) << " " << method.Name;

				WindowLayout::Layout::Treenode(ss.str().c_str(), scriptFlags, [&]() {
					ImGui::Text("Attributes:");

					TryProcessAttribute<Hazard::Scripting::VisualFuncAttribute>(method);
					TryProcessAttribute<Hazard::Scripting::ExposeFuncAttribute>(method);

					});
			}
			});
	}
	void ScriptEngineDebug::OnWindowRender()
	{
		HZR_PROFILE_FUNCTION();
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

		for (auto& [type, scriptEngine] : Hazard::Scripting::ScriptCommand::GetEngines()) {

			Hazard::Scripting::ScriptRegistry registry = scriptEngine->GetRegistry();

			WindowLayout::Layout::Treenode(ScriptTypeToString(type), flags, [&]() {
				for (auto& [moduleName, script] : registry.GetScripts())
				{
					Hazard::Scripting::ScriptMetadata sc = script;
					ImGuiTreeNodeFlags scriptFlags = ImGuiTreeNodeFlags_Framed;

					WindowLayout::Layout::Treenode(script.ClassName.c_str(), scriptFlags, [&]() {
						ImGui::Text("Count: %u", sc.Count);
						ImGui::Text("Attributes:");

						TryProcessAttribute<Hazard::Scripting::AuthorAttribute>(sc);
						TryProcessAttribute<Hazard::Scripting::TodoAttribute>(sc);

						ProcessFields(sc.Fields);
						ProcessMethods(sc.Methods);
						});
				}
				});
		}
	}
	template<typename T>
	void ScriptEngineDebug::TryProcessAttribute(Hazard::Scripting::ScriptMetadata meta)
	{
		if (meta.Has<T>()) {
			const T& attribute = meta.Get<T>();
			ProcessAttribute<T>(attribute);
		}
	}
	template<typename T>
	void ScriptEngineDebug::TryProcessAttribute(Hazard::Scripting::ScriptFieldMetadata field)
	{
		if (field.Has<T>()) {
			const T& attribute = field.Get<T>();
			ProcessAttribute<T>(attribute);
		}
	}
	template<typename T>
	void ScriptEngineDebug::TryProcessAttribute(Hazard::Scripting::ScriptMethodMetadata method)
	{
		if (method.Has<T>()) {
			const T& attribute = method.Get<T>();
			ProcessAttribute<T>(attribute);
		}
	}
	template<typename T>
	void ScriptEngineDebug::ProcessAttribute(T attribute) {
		assert(false);
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::AuthorAttribute attribute)
	{
		ImGui::Text("[Author] %s, Version: %.2f", attribute.Author.c_str(), attribute.Version);
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::TodoAttribute attribute)
	{
		ImGui::Text("{Todo]: %s, Progress: %s", attribute.Detail.c_str(), Scripting::Utils::ProgressToString(attribute.Progress));
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::ShowInPropertiesAttribute attribute)
	{
		ImGui::Text("[HideInProperties]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::HideInPropertiesAttribute attribute)
	{
		ImGui::Text("[ShowInProperties]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::SliderAttribute attribute)
	{
		ImGui::Text("[Slider] Min: %.2f, Max: %.2f", attribute.Min, attribute.Max);
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::RangeAttribute attribute)
	{
		ImGui::Text("[Range]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::HeaderAttribute attribute)
	{
		ImGui::Text("[Header]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::TooltipAttribute attribute)
	{
		ImGui::Text("[Tooltip]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::TextAreaAttribute attribute)
	{
		ImGui::Text("[TextArea]");
	}

	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::VisualFuncAttribute attribute)
	{
		ImGui::Text("[VisualFunc]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(Hazard::Scripting::ExposeFuncAttribute attribute)
	{
		ImGui::Text("[ExposeFunc]");
	}
}
