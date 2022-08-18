
#include <hzrpch.h>
#include "ScriptSerializer.h"
#include "MonoUtilities.h"

using namespace HazardScript;

namespace Hazard
{
	void ScriptSerializer::SerializeFieldEditor(YAML::Emitter& out, FieldMetadata* field, Ref<ScriptObject> object)
	{
		const ManagedType& elementType = field->GetType().IsArray() ? field->GetType().GetElementType() : field->GetType();


		if (field->GetType().IsArray())
		{
			YamlUtils::Map(out, field->GetName(), [&]() {
				for (size_t i = 0; i < object->GetFieldValueCount(field->GetName()); i++) {
					std::string key = std::to_string(i);
					SerializeFieldValue(out, field->GetName(), key, elementType.NativeType, object);
				}
				});
			return;
		}
		SerializeFieldValue(out, field->GetName(), field->GetName(), elementType.NativeType, object);
	}
	void ScriptSerializer::SerializeFieldValue(YAML::Emitter& out, const std::string& name, const std::string& key, const NativeType& type, Ref<HazardScript::ScriptObject> object)
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
}
