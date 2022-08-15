#pragma once

#include "Hazard/Scripting/Attributes/AllAttributes.h"
#include "HazardScript.h"

namespace UI
{
	/*
	static bool WillBeVisible(HazardScript::ScriptField& field) {
		using namespace HazardScript;

		if (field.Has<HideInPropertiesAttribute>()) return false;
		if (field.Has<ShowInPropertiesAttribute>()) return true;

		return true;
	}

	template<typename T>
	static bool ScriptField(HazardScript::ManagedField& field) {
		static_assert(false);
	}
	template<>
	static bool ScriptField<float>(HazardScript::ManagedField& field) 
	{
		using namespace HazardScript;
		float value = field.GetValue<float>();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		if (field.Field->Has<RangeAttribute>()) {
			RangeAttribute attrib = field.Field->Get<RangeAttribute>();
			modified = ImUI::InputFloat(value, 0.0f, attrib.Min, attrib.Max);
		}
		else if (field.Field->Has<SliderAttribute>()) {
			SliderAttribute attrib = field.Field->Get<SliderAttribute>();
			modified = ImUI::InputSliderFloat(value, 0.0f, attrib.Min, attrib.Max);
		}
		else
		{
			modified = ImUI::InputFloat(value, 0.0f);
		}

		if (modified) {
			//obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}

	template<>
	static bool ScriptField<int>(HazardScript::ManagedField& field) {
		using namespace HazardScript;
		int value = 0; // obj.GetFieldValue<int>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt(value, 0);

		if (modified) {
			//obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}

	template<>
	static bool ScriptField<uint64_t>(HazardScript::ManagedField& field) {
		using namespace HazardScript;
		uint64_t value = 0; //obj.GetFieldValue<uint64_t>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputUInt(value, 0);

		if (modified) {
			//obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}



	template<>
	static bool ScriptField<glm::vec2>(HazardScript::ManagedField& field) {
		glm::vec2 value; //obj.GetFieldValue<glm::vec2>(field.GetName());

		bool modified = ImUI::InputFloat2(value, 0.0f);
		if (modified) {
			//obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec3>(HazardScript::ManagedField& field) {
		glm::vec3 value; // = obj.GetFieldValue<glm::vec3>(field.GetName());
		bool modified = ImUI::InputFloat3(value, 0.0f);
		if (modified) {
			//obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec4>(HazardScript::ManagedField& field) {
		Color value;// = Color::FromGLM(obj.GetFieldValue<glm::vec4>(field.GetName()));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = ImUI::ColorPicker("##Tint", value);

		if (modified) {
			//obj.SetFieldValue<glm::vec4>(field.GetName(), value);
		}
		return modified;
	}

	template<>
	static bool ScriptField<std::string>(HazardScript::ManagedField& field) {
		using namespace HazardScript;
		std::string value;// = obj.GetFieldValue<std::string>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());


		bool modified = false;

		if (field.Field->Has<TextAreaAttribute>()) {
			TextAreaAttribute attrib = field.Field->Get<TextAreaAttribute>();
			modified = ImUI::TextArea(value, attrib.Min, attrib.Max);
		}
		else
			modified = ImUI::TextField(value);

		if (modified) {
			//obj.SetFieldValue<std::string>(field.GetName(), value);
		}
		return modified;
	}

	template<>
	static bool ScriptField<HazardScript::ScriptField>(HazardScript::ManagedField& field) {

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		//bool hasValue = field.HasValue();

		ImUI::TextField(field.Field->GetName());

		if (modified)
		{
			//obj.SetFieldValue<UID>(field.GetName(), value);
		}
		return modified;
	}
	*/

	static void ScriptField(const std::string& name, HazardScript::ManagedField& field)
	{
		using namespace HazardScript;
		//if (!WillBeVisible()) return;

		std::string label = name;
		label[0] = toupper(name[0]);
		ImGui::Text(label.c_str());
		//if (field.Field->Has<TooltipAttribute>()) 
		//{
		//	ImUI::Tooltip(field.Field->Get<TooltipAttribute>().Tooltip.c_str());
		//}
		ImGui::NextColumn();
		
		/*switch (field.Type.NativeType) {
		case NativeType::Float:			ScriptField<float>(field);						break;
		case NativeType::Float2:		ScriptField<glm::vec2>(field);					break;
		case NativeType::Float3:		ScriptField<glm::vec3>(field);					break;
		case NativeType::Float4:		ScriptField<glm::vec4>(field);					break;
		case NativeType::Int8:			ScriptField<int>(field);						break;
		case NativeType::UInt8:			ScriptField<uint64_t>(field);					break;
		case NativeType::String:		ScriptField<std::string>(field);				break;
		}
		*/
		
		ImGui::NextColumn();
	}
}