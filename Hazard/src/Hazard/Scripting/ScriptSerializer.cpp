
#include <hzrpch.h>
#include "ScriptSerializer.h"
#include "MonoUtilities.h"
#include "Attributes/AllAttributes.h"

using namespace HazardScript;

namespace Hazard
{
	void ScriptSerializer::SerializeFieldEditor(Ref<HazardScript::ScriptObject> object, YAML::Emitter& out, Ref<FieldMetadata> field)
	{
		HZR_PROFILE_FUNCTION();
		if (!(field->GetFlags() & MonoFlags_Public || field->Has<ShowInPropertiesAttribute>())) 
		{
			__debugbreak();
			return;
		}


		const ManagedType& elementType = field->GetType().IsArray() ? field->GetType().GetElementType() : field->GetType();

		if (field->GetType().IsArray())
		{
			YamlUtils::Map(out, field->GetName(), [&]() {
				for (size_t i = 0; i < object->GetFieldValueCount(field->GetName()); i++) {
					std::string key = std::to_string(i);
					SerializeFieldValue(out, field->GetName(), key, elementType.NativeType, object.Raw());
				}
				});
			return;
		}
		SerializeFieldValue(out, field->GetName(), field->GetName(), elementType.NativeType, object.Raw());
	}
	void ScriptSerializer::SerializeFieldValue(YAML::Emitter& out, const std::string& name, const std::string& key, const NativeType& type,HazardScript::ScriptObject* object)
	{
		switch (type)
		{
		case NativeType::Bool:
			YamlUtils::Serialize(out, key, object->GetFieldValue<bool>(name));
			break;
		case NativeType::Double:
			YamlUtils::Serialize(out, key, object->GetFieldValue<double>(name));
			break;
		case NativeType::Float:
			YamlUtils::Serialize(out, key, object->GetFieldValue<float>(name));
			break;
		case NativeType::Float2:
			YamlUtils::Serialize(out, key, object->GetFieldValue<glm::vec2>(name));
			break;
		case NativeType::Float3:
			YamlUtils::Serialize(out, key, object->GetFieldValue<glm::vec3>(name));
			break;
		case NativeType::Float4:
			YamlUtils::Serialize(out, key, object->GetFieldValue<glm::vec4>(name));
			break;
		case NativeType::Int8:
			YamlUtils::Serialize(out, key, object->GetFieldValue<int8_t>(name));
			break;
		case NativeType::Int16:
			YamlUtils::Serialize(out, key, object->GetFieldValue<int16_t>(name));
			break;
		case NativeType::Int32:
			YamlUtils::Serialize(out, key, object->GetFieldValue<int32_t>(name));
			break;
		case NativeType::Int64:
			YamlUtils::Serialize(out, key, object->GetFieldValue<int64_t>(name));
			break;
		case NativeType::UInt8:
			YamlUtils::Serialize(out, key, object->GetFieldValue<uint8_t>(name));
			break;
		case NativeType::UInt16:
			YamlUtils::Serialize(out, key, object->GetFieldValue<uint16_t>(name));
			break;
		case NativeType::UInt32:
			YamlUtils::Serialize(out, key, object->GetFieldValue<uint32_t>(name));
			break;
		case NativeType::UInt64:
			YamlUtils::Serialize(out, key, object->GetFieldValue<uint64_t>(name));
			break;
		case NativeType::String:
			YamlUtils::Serialize(out, key, object->GetFieldValue<std::string>(name));
			break;
		case NativeType::Reference:
			YamlUtils::Serialize(out, key, (uint64_t)object->GetFieldValue<ObjectReference>(name).ObjectUID);
			break;
		default:
			std::cout << std::endl;
			break;
		}
	}
	void ScriptSerializer::DeserializeFieldEditor(Ref<HazardScript::ScriptObject> object, const std::string& name, YAML::Node& valueNode)
	{
		HZR_PROFILE_FUNCTION();
		ScriptMetadata& script = object->GetScript();
		if (!script.HasField(name)) return;

		FieldMetadata& field = script.GetField(name);
		const ManagedType& elementType = field.GetType().IsArray() ? field.GetType().GetElementType() : field.GetType();

		DeserializeFieldValue(name, object, valueNode, elementType.NativeType);
	}
	void ScriptSerializer::DeserializeFieldValue(const std::string& name, Ref<HazardScript::ScriptObject> object, YAML::Node& valueNode, const NativeType& type)
	{
		switch (type)
		{
		case NativeType::Bool:
			object->SetFieldValue<bool>(name, valueNode.as<bool>(), 0);
			break;
		case NativeType::Double:
			object->SetFieldValue<double>(name, valueNode.as<double>(), 0);
			break;
		case NativeType::Float:
			object->SetFieldValue<float>(name, valueNode.as<float>(), 0);
			break;
		case NativeType::Float2:
			object->SetFieldValue<glm::vec2>(name, valueNode.as<glm::vec2>(), 0);
			break;
		case NativeType::Float3:
			object->SetFieldValue<glm::vec3>(name, valueNode.as<glm::vec3>(), 0);
			break;
		case NativeType::Float4:
			object->SetFieldValue<glm::vec4>(name, valueNode.as<glm::vec4>(), 0);
			break;
		case NativeType::Int8:
			object->SetFieldValue<int8_t>(name, valueNode.as<int8_t>(), 0);
			break;
		case NativeType::Int16:
			object->SetFieldValue<int16_t>(name, valueNode.as<int16_t>(), 0);
			break;
		case NativeType::Int32:
			object->SetFieldValue<int32_t>(name, valueNode.as<int32_t>(), 0);
			break;
		case NativeType::Int64:
			object->SetFieldValue<int64_t>(name, valueNode.as<int64_t>(), 0);
			break;
		case NativeType::UInt8:
			object->SetFieldValue<uint8_t>(name, valueNode.as<uint8_t>(), 0);
			break;
		case NativeType::UInt16:
			object->SetFieldValue<uint16_t>(name, valueNode.as<uint16_t>(), 0);
			break;
		case NativeType::UInt32:
			object->SetFieldValue<uint32_t>(name, valueNode.as<uint32_t>(), 0);
			break;
		case NativeType::UInt64:
			object->SetFieldValue<uint64_t>(name, valueNode.as<uint64_t>(), 0);
			break;
		case NativeType::String:
			object->SetFieldValue<std::string>(name, valueNode.as<std::string>(), 0);
			break;
		case NativeType::Reference:
		{
			ObjectReference ref;
			ref.ObjectUID = valueNode.as<uint64_t>();
			object->SetFieldValue<ObjectReference>(name, ref, 0);
			break;
		}
		default:
			HZR_CORE_ASSERT(false, name);
			break;
		}
	}
}
