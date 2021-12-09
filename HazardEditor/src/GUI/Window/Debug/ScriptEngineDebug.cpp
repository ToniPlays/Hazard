#pragma once

#include "ScriptEngineDebug.h"

#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"
#include "GUI/EditorView.h"
#include "GUI/Window/Tabs/EditorMainTab.h"
#include "Hazard/Scripting/ScriptRegistry.h"
#include "Hazard/Scripting/Attributes/AllAttributes.h"

using namespace WindowLayout;
using namespace Hazard::Scripting;

namespace WindowElement
{
	ScriptEngineDebug::ScriptEngineDebug() : EditorWindow("Script engine debugger")
	{

	}
	void ScriptEngineDebug::Init()
	{

	}
	void ScriptEngineDebug::ProcessFields(std::vector<ScriptFieldMetadata> fields)
	{
		ImGuiTreeNodeFlags scriptFlags = ImGuiTreeNodeFlags_Framed;
		Layout::Treenode("Class fields", scriptFlags, [&]() {
			for (auto& field : fields)
			{
				std::stringstream ss;
				ss << FieldVisibilityToChar(field.Visibility) << " ";
				ss << FieldTypeToChar(field.Type) << " " << field.Name;

				Layout::Treenode(ss.str().c_str(), scriptFlags, [&]() {
					ImGui::Text("Attributes:");

					TryProcessAttribute<ShowInPropertiesAttribute>(field);
					TryProcessAttribute<HideInPropertiesAttribute>(field);
					TryProcessAttribute<SliderAttribute>(field);
					TryProcessAttribute<RangeAttribute>(field);
					TryProcessAttribute<HeaderAttribute>(field);
					TryProcessAttribute<TooltipAttribute>(field);
					TryProcessAttribute<TextAreaAttribute>(field);

				});
			}
			});
	}
	void ScriptEngineDebug::ProcessMethods(std::vector<ScriptMethodMetadata> methods)
	{
		ImGuiTreeNodeFlags scriptFlags = ImGuiTreeNodeFlags_Framed;
		Layout::Treenode("Class methods", scriptFlags, [&]() {
			for (auto& method : methods)
			{
				std::stringstream ss;
				ss << FieldVisibilityToChar(method.Visibility) << " " << method.Name;

				Layout::Treenode(ss.str().c_str(), scriptFlags, [&]() {
					ImGui::Text("Attributes:");

					TryProcessAttribute<VisualFuncAttribute>(method);
					TryProcessAttribute<ExposeFuncAttribute>(method);

					});
			}
			});
	}
	void ScriptEngineDebug::OnWindowRender()
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

		for (auto& [type, scriptEngine] : ScriptCommand::GetEngines()) {

			ScriptRegistry registry = scriptEngine->GetRegistry();

			Layout::Treenode(ScriptTypeToString(type), flags, [&]() {
				for (auto& [moduleName, script] : registry.GetScripts())
				{
					ScriptMetadata sc = script;
					ImGuiTreeNodeFlags scriptFlags = ImGuiTreeNodeFlags_Framed;

					Layout::Treenode(script.ClassName.c_str(), scriptFlags, [&]() {
						ImGui::Text("Count: %u", sc.Count);
						ImGui::Text("Attributes:");

						TryProcessAttribute<AuthorAttribute>(sc);
						TryProcessAttribute<TodoAttribute>(sc);

						ProcessFields(sc.Fields);
						ProcessMethods(sc.Methods);
						});
				}
				});
		}
	}
	template<typename T>
	void ScriptEngineDebug::TryProcessAttribute(ScriptMetadata meta)
	{
		if (meta.Has<T>()) {
			T& attribute = meta.Get<T>();
			ProcessAttribute<T>(attribute);
		}
	}
	template<typename T>
	void ScriptEngineDebug::TryProcessAttribute(ScriptFieldMetadata field)
	{
		if (field.Has<T>()) {
			T& attribute = field.Get<T>();
			ProcessAttribute<T>(attribute);
		}
	}
	template<typename T>
	void ScriptEngineDebug::TryProcessAttribute(ScriptMethodMetadata method)
	{
		if (method.Has<T>()) {
			T& attribute = method.Get<T>();
			ProcessAttribute<T>(attribute);
		}
	}
	template<typename T>
	void ScriptEngineDebug::ProcessAttribute(T attribute) {
		static_assert(false);
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(AuthorAttribute attribute)
	{
		ImGui::Text("[Author] %s, Version: %.2f", attribute.Author.c_str(), attribute.Version);
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(TodoAttribute attribute)
	{
		ImGui::Text("{Todo]: %s, Progress: %s", attribute.Detail.c_str(), Scripting::Utils::ProgressToString(attribute.Progress));
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(ShowInPropertiesAttribute attribute)
	{
		ImGui::Text("[HideInProperties]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(HideInPropertiesAttribute attribute)
	{
		ImGui::Text("[ShowInProperties]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(SliderAttribute attribute)
	{
		ImGui::Text("[Slider] Min: %.2f, Max: %.2f", attribute.Min, attribute.Max);
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(RangeAttribute attribute)
	{
		ImGui::Text("[Range]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(HeaderAttribute attribute)
	{
		ImGui::Text("[Header]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(TooltipAttribute attribute)
	{
		ImGui::Text("[Tooltip]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(TextAreaAttribute attribute)
	{
		ImGui::Text("[TextArea]");
	}

	template<>
	void ScriptEngineDebug::ProcessAttribute(VisualFuncAttribute attribute)
	{
		ImGui::Text("[VisualFunc]");
	}
	template<>
	void ScriptEngineDebug::ProcessAttribute(ExposeFuncAttribute attribute)
	{
		ImGui::Text("[ExposeFunc]");
	}
}