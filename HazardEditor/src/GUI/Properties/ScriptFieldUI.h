#pragma once

#include "Hazard/Scripting/Attributes/AllAttributes.h"
#include "HazardScript.h"
#include "Hazard/Scripting/MonoUtilities.h"

namespace UI
{
	static bool WillBeVisible(Ref<HazardScript::FieldMetadata> field)
	{
		using namespace HazardScript;
		if (field->Has<HideInPropertiesAttribute>()) return false;
		if (field->Has<ShowInPropertiesAttribute>()) return true;

		return field->GetFlags() & MonoFlags_Public;
	}

	template<typename T>
	static bool ScriptField(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) 
	{
		STATIC_ASSERT(false, T);
	}

	template<>
	static bool ScriptField<bool>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world)
	{
		using namespace HazardScript;
		bool value = obj->GetFieldValue<bool>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		modified = ImGui::Checkbox("##bool", &value);

		if (modified) 
			obj->SetFieldValue(field->GetName(), value, index);

		return modified;
	}


	template<>
	static bool ScriptField<float>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world)
	{
		using namespace HazardScript;
		float value = obj->GetFieldValue<float>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (field->Has<RangeAttribute>()) 
		{
			Ref<RangeAttribute> attrib = field->Get<RangeAttribute>();
			modified = ImUI::InputFloat(value, 0.0f, attrib->Min, attrib->Max);
		}
		else if (field->Has<SliderAttribute>()) 
		{
			Ref<SliderAttribute> attrib = field->Get<SliderAttribute>();
			modified = ImUI::SliderFloat(value, 0.0f, attrib->Min, attrib->Max);
		}
		else
		{
			modified = ImUI::InputFloat(value, 0.0f);
		}

		if (modified) 
			obj->SetFieldValue(field->GetName(), value, index);

		return modified;
	}

	template<>
	static bool ScriptField<double>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		double value = obj->GetFieldValue<double>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputDouble(value, 0.0f);

		if (modified) 
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}

	template<>
	static bool ScriptField<int8_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		int8_t value = obj->GetFieldValue<int8_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);

		return modified;
	}
	template<>
	static bool ScriptField<int16_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		int16_t value = obj->GetFieldValue<int16_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<int32_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		int32_t value = obj->GetFieldValue<int32_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<int64_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		uint64_t value = obj->GetFieldValue<int64_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}

	template<>
	static bool ScriptField<uint8_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		uint8_t value = obj->GetFieldValue<uint8_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<uint16_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		uint16_t value = obj->GetFieldValue<uint16_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<uint32_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		uint32_t value = obj->GetFieldValue<uint32_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<uint64_t>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		uint64_t value = obj->GetFieldValue<uint64_t>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = false;

		modified = ImUI::InputInt((int&)value, 0);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}

	template<>
	static bool ScriptField<glm::vec2>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		glm::vec2 value = obj->GetFieldValue<glm::vec2>(field->GetName());

		bool modified = ImUI::InputFloat2(value, 0.0f);
		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec3>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		glm::vec3 value = obj->GetFieldValue<glm::vec3>(field->GetName(), index);
		bool modified = ImUI::InputFloat3(value, 0.0f);

		if (modified)
			obj->SetFieldValue(field->GetName(), value, index);
		return modified;
	}
	template<>
	static bool ScriptField<glm::vec4>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		Color value = Color::FromGLM(obj->GetFieldValue<glm::vec4>(field->GetName(), index));
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = ImUI::ColorPicker("##color", value);
		if (modified) {
			obj->SetFieldValue<glm::vec4>(field->GetName(), value.ToGLM(), index);
		}
		return modified;
	}
	template<>
	static bool ScriptField<std::string>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {
		using namespace HazardScript;
		std::string value = obj->GetFieldValue<std::string>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		bool modified = false;

		if (field->Has<TextAreaAttribute>()) 
		{
			Ref<TextAreaAttribute> attrib = field->Get<TextAreaAttribute>();
			modified = ImUI::TextArea(value, attrib->Min, attrib->Max);
		}
		else modified = ImUI::TextField(value);

		if (modified)
			obj->SetFieldValue<std::string>(field->GetName(), value, index);
		return modified;
	}

	template<>
	static bool ScriptField<Hazard::ObjectReference>(uint32_t index, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world) {

		using namespace HazardScript;
		Hazard::ObjectReference value = obj->GetFieldValue<Hazard::ObjectReference>(field->GetName(), index);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		std::string typeName;
		std::string displayName;
		std::string tag;

		if (field->GetType().IsArray())
		{
			typeName = field->GetType().GetElementType().FullName;
			displayName = field->GetType().GetElementType().DisplayName;
		}
		else
		{
			typeName = field->GetType().FullName;
			displayName = field->GetType().DisplayName;
		}

		bool isObjectReference = true;

		if (Hazard::Utils::ScriptClassToAsset(typeName) == AssetType::Undefined) 
		{
			Entity referenced = world->TryGetEntityFromUUID(value.ObjectUID);
			tag = referenced.IsValid() ? referenced.GetTag().Tag : "";
		}
		else
		{
			const AssetMetadata& metadata = AssetManager::GetMetadata(value.ObjectUID);
			if (metadata.Handle != INVALID_ASSET_HANDLE)
				tag = File::GetNameNoExt(metadata.Path);
			isObjectReference = false;
		}

		std::string text = (value.ObjectUID != 0 ? tag : "None") + " (" + displayName + ")";
		float rowHeight = ImGui::GetTextLineHeightWithSpacing();
		ImGui::Text(text.c_str());

		bool modified = false;
		if (isObjectReference) 
		{
			ImUI::DropTarget<UID>("Hazard.Entity", [&](UID uid) {
				value.ObjectUID = uid;
				modified |= true;
				});
			ImUI::DropTarget<UID>(typeName.c_str(), [&](UID uid) {
				value.ObjectUID = uid;
				modified |= true;
				});
		}
		ImUI::DropTarget<AssetHandle>(Hazard::Utils::ScriptClassToAsset(typeName), [&](UID uid) {
			value.ObjectUID = uid;
			modified |= true;
			});


		ImGui::SameLine(ImGui::GetContentRegionAvail().x, 5);
		if (ImGui::Button("X", { rowHeight, rowHeight })) {
			value.ObjectUID = 0;
			modified |= true;
		}

		if (modified) 
			obj->SetFieldValue<Hazard::ObjectReference>(field->GetName(), value, index);

		return modified;
	}

	static void ScriptField(const std::string& name, Ref<HazardScript::FieldMetadata> field, Ref<HazardScript::ScriptObject> obj, Ref<World> world)
	{
		using namespace HazardScript;
		if (!WillBeVisible(field)) return;

		std::string label = name;
		label[0] = toupper(name[0]);
		ImGui::Text(label.c_str());

		if (field->Has<TooltipAttribute>())
		{
			ImUI::Tooltip(field->Get<TooltipAttribute>()->Tooltip.c_str());
		}
		ImGui::NextColumn();

		const ManagedType& elementType = field->GetType().IsArray() ? field->GetType().GetElementType() : field->GetType();

		if (field->GetType().IsArray()) 
		{
			uint32_t size = obj->GetFieldValueCount(field->GetName());
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			bool resized = ImUI::InputInt((int&)size, 0);

			if (resized) 
			{
				obj->SetArraySize(field->GetName(), size);
			}
		}

		switch (elementType.NativeType)
		{
		case NativeType::Bool:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<bool>(i, field, obj, world);
					});
			break;
		case NativeType::Float:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<float>(i, field, obj, world);
					});
			break;
		case NativeType::Double:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<double>(i, field, obj, world);
					});
			break;
		case NativeType::Float2:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<glm::vec2>(i, field, obj, world);
					});
			break;
		case NativeType::Float3:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<glm::vec3>(i, field, obj, world);
					});
			break;
		case NativeType::Float4:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<glm::vec4>(i, field, obj, world);
					});
			break;
		case NativeType::Int8:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int8_t>(i, field, obj, world);
					});
			break;
		case NativeType::Int16:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int16_t>(i, field, obj, world);
					});
			break;
		case NativeType::Int32:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int32_t>(i, field, obj, world);
					});
			break;
		case NativeType::Int64:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<int64_t>(i, field, obj, world);
					});
			break;

		case NativeType::UInt8:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint8_t>(i, field, obj, world);
					});
			break;
		case NativeType::UInt16:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint16_t>(i, field, obj, world);
					});
			break;
		case NativeType::UInt32:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint32_t>(i, field, obj, world);
					});
			break;
		case NativeType::UInt64:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<uint64_t>(i, field, obj, world);
					});
			break;

		case NativeType::String:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
				ImUI::Group(std::to_string(i).c_str(), [&]() {
				ScriptField<std::string>(i, field, obj, world);
					});
			break;
		case NativeType::Reference:
			for (uint32_t i = 0; i < obj->GetFieldValueCount(field->GetName()); i++)
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