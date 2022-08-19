#pragma once

#include "Hazard/Scripting/Attributes/AllAttributes.h"
#include "HazardScript.h"
#include "Hazard/Scripting/MonoUtilities.h"

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
	static bool ScriptField(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		static_assert(false);
	}

	template<>
	static bool ScriptField<bool>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		bool value = obj.GetFieldValue<bool>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		modified = ImGui::Checkbox("##bool", &value);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}

		return modified;
	}


	template<>
	static bool ScriptField<float>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		float value = obj.GetFieldValue<float>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
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
			obj.SetFieldValue(field.GetName(), value, index);
		}

		return modified;
	}

	template<>
	static bool ScriptField<double>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		double value = obj.GetFieldValue<double>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputDouble(value, 0.0f);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}

	template<>
	static bool ScriptField<int8_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		int8_t value = obj.GetFieldValue<int8_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<int16_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		int16_t value = obj.GetFieldValue<int16_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<int32_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		int32_t value = obj.GetFieldValue<int32_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<int64_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		uint64_t value = obj.GetFieldValue<int64_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}

	template<>
	static bool ScriptField<uint8_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		uint8_t value = obj.GetFieldValue<uint8_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<uint16_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		uint16_t value = obj.GetFieldValue<uint16_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<uint32_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		uint32_t value = obj.GetFieldValue<uint32_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<uint64_t>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		uint64_t value = obj.GetFieldValue<uint64_t>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0.0);

		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}

	template<>
	static bool ScriptField<glm::vec2>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		glm::vec2 value = obj.GetFieldValue<glm::vec2>(field.GetName());

		bool modified = ImUI::InputFloat2(value, 0.0f);
		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec3>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		glm::vec3 value = obj.GetFieldValue<glm::vec3>(field.GetName(), index);
		bool modified = ImUI::InputFloat3(value, 0.0f);
		if (modified) {
			obj.SetFieldValue(field.GetName(), value, index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec4>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		Color value = Color::FromGLM(obj.GetFieldValue<glm::vec4>(field.GetName(), index));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = ImUI::ColorPicker("##color", value);
		if (modified) {
			obj.SetFieldValue<glm::vec4>(field.GetName(), value.ToGLM(), index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<std::string>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {
		using namespace HazardScript;
		std::string value = obj.GetFieldValue<std::string>(field.GetName(), index);
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
			obj.SetFieldValue<std::string>(field.GetName(), value, index);
		}
		return modified;
	}

	template<>
	static bool ScriptField<Hazard::ObjectReference>(uint32_t index, HazardScript::FieldMetadata& field, HazardScript::ScriptObject& obj, Ref<World> world) {

		using namespace HazardScript;
		Hazard::ObjectReference value = obj.GetFieldValue<Hazard::ObjectReference>(field.GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());

		std::string typeName = field.GetType().IsArray() ? field.GetType().GetElementType().GetTypeName() : field.GetType().GetTypeName();
		Entity referenced = world->GetEntityFromUID(value.ObjectUID);
		std::string& tag = referenced.IsValid() ? referenced.GetTag().Tag : "";

		std::string text = (value.ObjectUID != 0 ? tag : "None") + " (" + typeName + ")";
		ImGui::Text(text.c_str());

		bool modified = false;

		ImUI::DropTarget<UID>(typeName.c_str(), [&](UID uid) {
			value.ObjectUID = uid;
			modified = true;
			});

		if (modified) {
			obj.SetFieldValue<Hazard::ObjectReference>(field.GetName(), value, index);
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

		if (field.Has<TooltipAttribute>())
		{
			ImUI::Tooltip(field.Get<TooltipAttribute>().Tooltip.c_str());
		}
		ImGui::NextColumn();

		const ManagedType& elementType = field.GetType().IsArray() ? field.GetType().GetElementType() : field.GetType();

		if (field.GetType().IsArray()) {
			uint32_t size = obj.GetFieldValueCount(field.GetName());
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			bool resized = ImUI::InputInt((int&)size, 0);

			if (resized) {
				obj.SetArraySize(field.GetName(), size);
			}
		}

		switch (elementType.NativeType)
		{
		case NativeType::Bool:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<bool>(i, field, obj, world);
					});
			break;
		case NativeType::Float:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<float>(i, field, obj, world);
					});
			break;
		case NativeType::Double:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<double>(i, field, obj, world);
					});
			break;
		case NativeType::Float2:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<glm::vec2>(i, field, obj, world);
					});
			break;
		case NativeType::Float3:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<glm::vec3>(i, field, obj, world);
					});
			break;

		case NativeType::Float4:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<glm::vec4>(i, field, obj, world);
					});
			break;
		case NativeType::Int8:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int8_t>(i, field, obj, world);
					});
			break;
		case NativeType::Int16:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int16_t>(i, field, obj, world);
					});
			break;
		case NativeType::Int32:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int32_t>(i, field, obj, world);
					});
			break;
		case NativeType::Int64:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int64_t>(i, field, obj, world);
					});
			break;

		case NativeType::UInt8:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint8_t>(i, field, obj, world);
					});
			break;
		case NativeType::UInt16:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint16_t>(i, field, obj, world);
					});
			break;
		case NativeType::UInt32:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint32_t>(i, field, obj, world);
					});
			break;
		case NativeType::UInt64:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint64_t>(i, field, obj, world);
					});
			break;

		case NativeType::String:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<std::string>(i, field, obj, world);
					});
			break;
		case NativeType::Reference:
			for (size_t i = 0; i < obj.GetFieldValueCount(field.GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<Hazard::ObjectReference>(i, field, obj, world);
					});
			break;

			//default:
			//	HZR_ASSERT(false, "Wooooop");
			//	break;
			//}
		}
		ImGui::NextColumn();
	}
}