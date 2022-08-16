#pragma once

#include "Hazard/Scripting/Attributes/AllAttributes.h"
#include "HazardScript.h"

namespace UI
{
	static bool WillBeVisible(HazardScript::FieldMetadata& field) 
	{
		using namespace HazardScript;
		if (field.Has<HideInPropertiesAttribute>()) return false;
		if (field.Has<ShowInPropertiesAttribute>()) return true;

		return field.GetFlags() & MonoFlags_Public;
	}

	template<typename T>
	static bool ScriptField(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		static_assert(false);
	}
	template<>
	static bool ScriptField<float>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
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
	static bool ScriptField<double>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		double value = obj.GetFieldValue<double>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputDouble(value, 0.0f);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<uint64_t>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		uint64_t value = obj.GetFieldValue<uint64_t>(field.GetName());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		std::string typeName = field.GetType().GetTypeName();
		std::string text = (value != 0 ? world->GetEntityFromUID(value).GetTag().Tag : "None") + " (" + typeName + ")";

		ImGui::Text(text.c_str(), 0.0f);
		ImUI::DropTarget<UID>(typeName.c_str(), [&](UID uid) {
			value = (uint64_t)uid;
			modified = true;
			});
		ImUI::DropTarget<UID>("Hazard.Component", [&](UID uid) {
			value = (uint64_t)uid;
			modified = true;
			});

		if (modified) 
			obj.SetFieldValue(field.GetName(), value);
		return modified;
	}

	template<>
	static bool ScriptField<glm::vec2>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		glm::vec2 value = obj.GetFieldValue<glm::vec2>(field.GetName());

		bool modified = ImUI::InputFloat2(value, 0.0f);
		if (modified) {
			obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec3>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		glm::vec3 value = obj.GetFieldValue<glm::vec3>(field.GetName());
		bool modified = ImUI::InputFloat3(value, 0.0f);
		if (modified) {
			obj.SetFieldValue(field.GetName(), value);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec4>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		Color value = Color::FromGLM(obj.GetFieldValue<glm::vec4>(field.GetName()));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = ImUI::ColorPicker("##color", value);
		if (modified) {	
			obj.SetFieldValue<glm::vec4>(field.GetName(), value.ToGLM());
		}
		return modified;
	}
	template<>
	static bool ScriptField<std::string>(HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
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

	static void ScriptField(const std::string& name, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world)
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

		switch (field.GetType().NativeType)
		{
		case NativeType::Float:		ScriptField<float>(field, obj, world);			break;
		case NativeType::Double:	ScriptField<double>(field, obj, world);			break;
		case NativeType::Float2:	ScriptField<glm::vec2>(field, obj, world);		break;
		case NativeType::Float3:	ScriptField<glm::vec3>(field, obj, world);		break;
		case NativeType::Float4:	ScriptField<glm::vec4>(field, obj, world);		break;
		case NativeType::String:	ScriptField<std::string>(field, obj, world);	break;
		case NativeType::Reference: ScriptField<uint64_t>(field, obj, world);		break;
			//default:
			//	HZR_ASSERT(false, "Wooooop");
			//	break;
			//}
		}
		ImGui::NextColumn();
	}
}