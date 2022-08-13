#pragma once

#include "HazardScript.h"

namespace UI
{

	static bool WillBeVisible(HazardScript::ScriptField& field) {
		using namespace HazardScript;

		if (field.Has<HideInPropertiesAttribute>()) return false;
		if (field.Has<ShowInPropertiesAttribute>()) return true;

		return field.GetVisibility() == FieldVisibility::Public;
	}

	template<typename T>
	static bool ScriptField(HazardScript::ScriptField& field, HazardScript::ScriptObject& obj) {
		static_assert(false);
	}
	template<>
	static bool ScriptField<float>(HazardScript::ScriptField& field, HazardScript::ScriptObject& obj) {
		using namespace HazardScript;
		float value = obj.GetFieldValue<float>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		if (field.Has<RangeAttribute>()) {
			RangeAttribute attrib = field.Get<RangeAttribute>();
			modified = ImUI::InputFloat(value, 0.0f, attrib.Min, attrib.Max);
		}
		else if (field.Has<SliderAttribute>()) {
			SliderAttribute attrib = field.Get<SliderAttribute>();
			modified = ImUI::InputSliderFloat(value, 0.0f, attrib.Min, attrib.Max);
		}
		else
		{
			modified = ImUI::InputFloat(value, 0.0f);
		}

		if (modified) {
			obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec2>(HazardScript::ScriptField& field, HazardScript::ScriptObject& obj) {
		glm::vec2 value = obj.GetFieldValue<glm::vec2>(field.GetName());

		bool modified = ImUI::InputFloat2(value, 0.0f);
		if (modified) {
			obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec3>(HazardScript::ScriptField& field, HazardScript::ScriptObject& obj) {
		glm::vec3 value = obj.GetFieldValue<glm::vec3>(field.GetName());
		bool modified = ImUI::InputFloat3(value, 0.0f);
		if (modified) {
			obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec4>(HazardScript::ScriptField& field, HazardScript::ScriptObject& obj) {
		Color value = Color::FromGLM(obj.GetFieldValue<glm::vec4>(field.GetName()));
		bool modified = ImUI::ColorPicker("#color", value);
		if (modified) {
			obj.SetFieldValue<glm::vec4>(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<std::string>(HazardScript::ScriptField& field, HazardScript::ScriptObject& obj) {
		using namespace HazardScript;
		std::string value = obj.GetFieldValue<std::string>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());


		bool modified = false;

		if (field.Has<TextAreaAttribute>()) {
			TextAreaAttribute attrib = field.Get<TextAreaAttribute>();
			modified = ImUI::TextArea(value, attrib.Min, attrib.Max);
		}
		else {
			modified = ImUI::TextField(value);
		}

		if (modified) {
			obj.SetFieldValue<std::string>(field.GetName(), value);
		}
		return modified;
	}

	static void ScriptField(const std::string& name, HazardScript::ScriptField& field, HazardScript::ScriptObject& obj)
	{
		using namespace HazardScript;
		if (!WillBeVisible(field)) return;


		std::string label = name;
		label[0] = toupper(name[0]);
		ImGui::Text(label.c_str());
		if (field.Has<TooltipAttribute>()) {
			ImUI::Tooltip(field.Get<TooltipAttribute>().Tooltip.c_str());
		}
		ImGui::NextColumn();

		switch (field.GetType()) {
		case FieldType::Float:  ScriptField<float>(field, obj);     break;
		case FieldType::Float2: ScriptField<glm::vec2>(field, obj);	break;
		case FieldType::Float3: ScriptField<glm::vec3>(field, obj);	break;
		case FieldType::Float4: ScriptField<glm::vec4>(field, obj);	break;
		case FieldType::Int: break;
		case FieldType::UInt: break;
		case FieldType::String: ScriptField<std::string>(field, obj); break;
		default:
			HZR_ASSERT(false, "Wooooop");
			break;
		}
		ImGui::NextColumn();
	}
}